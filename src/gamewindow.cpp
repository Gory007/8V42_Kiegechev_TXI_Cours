#include "gamewindow.h"
#include <QDebug>

GameWindow::GameWindow(QObject *parent) 
    : QObject(parent), selectedPosition(-1, -1), isPieceSelected(false) {}

void GameWindow::startNewGame(int mode, int playerColor) {
    qDebug() << "=== GameWindow::startNewGame ===";
    qDebug() << "Mode:" << mode << "PlayerColor:" << playerColor;
    GameMode gameMode = (mode == 0) ? GameMode::PvP : GameMode::PvAI;
    Color color = (playerColor == 0) ? Color::White : Color::Black;
    
    controller.startNewGame(gameMode, color);
    
    clearSelection();
    
    emit currentPlayerChanged();
    emit gameStateChanged();
    emit boardUpdated();
}

void GameWindow::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    Position from(fromRow, fromCol);
    Position to(toRow, toCol);
    
    if (controller.handleMove(from, to)) {
        clearSelection(); 
        
        GameState state = controller.checkGameState();
        
        if (state == GameState::Check) {
            emit messageShown("Шах!");
        } else if (state == GameState::Checkmate) {
            QString winner = (controller.getCurrentPlayer() == Color::White) ? "Чёрные" : "Белые";
            emit messageShown("Мат! Победили " + winner);
        } else if (state == GameState::Stalemate) {
            emit messageShown("Пат! Ничья");
        }
        
        emit currentPlayerChanged();
        emit gameStateChanged();
        emit boardUpdated();
        emit historyChanged();
    } else {
        emit messageShown("Некорректный ход!");
        clearSelection();  
        emit boardUpdated();
    }
}


void GameWindow::selectPiece(int row, int col) {
    qDebug() << "selectPiece called:" << row << col;
    
    // Проверяем, что фигура принадлежит текущему игроку
    Piece* piece = controller.board.getPiece(Position(row, col));
    if (piece && piece->color == controller.getCurrentPlayer()) {
        // Если уже есть выбранная фигура, снимаем выделение
        if (isPieceSelected) {
            isPieceSelected = false;
        }
        
        selectedPosition = Position(row, col);
        isPieceSelected = true;
        
        qDebug() << "Piece selected at:" << row << col;
        emit boardUpdated();  // Обновляем доску для показа легальных ходов
    } else {
        qDebug() << "Cannot select piece at:" << row << col 
                 << "- wrong color or no piece";
        clearSelection();
    }
}

void GameWindow::clearSelection() {
    qDebug() << "clearSelection called";
    selectedPosition = Position(-1, -1);
    isPieceSelected = false;
    emit boardUpdated();
}

QVariantList GameWindow::getBoardState() const {
    QVariantList boardState;
    
    // Получаем легальные ходы для выбранной фигуры
    std::vector<Position> legalMoves;
    if (isPieceSelected) {
        Piece* piece = controller.board.getPiece(selectedPosition);
        if (piece && piece->color == controller.getCurrentPlayer()) {
            // Получаем все возможные ходы фигуры
            std::vector<Position> allMoves = piece->getLegalMoves(controller.board);
            
            // Фильтруем ходы, которые не оставляют короля под шахом
            for (const auto& to : allMoves) {
                Board tempBoard = controller.board.clone();
                tempBoard.movePiece(selectedPosition, to);
                
                Position kingPos = tempBoard.findKing(controller.getCurrentPlayer());
                Color opponentColor = (controller.getCurrentPlayer() == Color::White) ? Color::Black : Color::White;
                
                if (!tempBoard.isSquareAttacked(kingPos, opponentColor)) {
                    legalMoves.push_back(to);
                }
            }
            
            qDebug() << "Legal moves for selected piece:" << legalMoves.size();
        } else {
            qDebug() << "No valid piece selected or wrong color";
            const_cast<GameWindow*>(this)->clearSelection();
        }
    }
    
    // Создаем ДВУМЕРНЫЙ массив 8x8
    for (int r = 0; r < 8; ++r) {
        QVariantList row;
        for (int c = 0; c < 8; ++c) {
            Piece* piece = controller.board.getPiece(Position(r, c));
            
            QVariantMap cellData;
            cellData["row"] = r;
            cellData["col"] = c;
            cellData["hasPiece"] = false;
            cellData["color"] = "";
            cellData["type"] = "";
            cellData["isSelected"] = false;
            cellData["isLegalMove"] = false;
            
            if (piece) {
                cellData["hasPiece"] = true;
                cellData["color"] = (piece->color == Color::White) ? "white" : "black";
                
                switch (piece->getType()) {
                    case PieceType::King:   cellData["type"] = "king"; break;
                    case PieceType::Queen:  cellData["type"] = "queen"; break;
                    case PieceType::Rook:   cellData["type"] = "rook"; break;
                    case PieceType::Bishop: cellData["type"] = "bishop"; break;
                    case PieceType::Knight: cellData["type"] = "knight"; break;
                    case PieceType::Pawn:   cellData["type"] = "pawn"; break;
                    default: cellData["type"] = ""; break;
                }
            }
            
            // Проверяем, выбрана ли эта клетка
            if (isPieceSelected && selectedPosition.row == r && selectedPosition.col == c) {
                cellData["isSelected"] = true;
            }
            
            // Проверяем, является ли клетка легальным ходом
            for (const auto& move : legalMoves) {
                if (move.row == r && move.col == c) {
                    cellData["isLegalMove"] = true;
                    break;
                }
            }
            
            row.append(cellData);
        }
        boardState.append(row);
    }
    
    qDebug() << "Board state rows:" << boardState.size();
    if (boardState.size() > 0) {
        qDebug() << "First row size:" << boardState[0].toList().size();
    }
    
    return boardState;
}

QVariantList GameWindow::getMoveHistory() const {
    QVariantList history;
    auto moves = controller.history.getAll();
    
    int moveNumber = 1;
    for (size_t i = 0; i < moves.size(); i += 2) {
        QVariantMap movePair;
        movePair["number"] = moveNumber;
        movePair["white"] = QString::fromStdString(moves[i].toString());
        
        if (i + 1 < moves.size()) {
            movePair["black"] = QString::fromStdString(moves[i + 1].toString());
        } else {
            movePair["black"] = "";
        }
        
        history.append(movePair);
        moveNumber++;
    }
    
    return history;
}

void GameWindow::saveGame(const QString& filename) {
    controller.saveHistoryToFile(filename.toStdString());
    emit messageShown("Игра сохранена в " + filename);
}

Color GameWindow::getCurrentPlayer() const {
    return controller.getCurrentPlayer();
}

GameState GameWindow::getGameState() const {
    return controller.checkGameState();
}