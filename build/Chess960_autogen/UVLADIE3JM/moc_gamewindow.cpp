/****************************************************************************
** Meta object code from reading C++ file 'gamewindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/gamewindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10GameWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto GameWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10GameWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameWindow",
        "currentPlayerChanged",
        "",
        "gameStateChanged",
        "messageShown",
        "message",
        "boardUpdated",
        "historyChanged",
        "networkGameChanged",
        "connectionStatusChanged",
        "status",
        "canMakeMoveChanged",
        "myColorChanged",
        "isConnectingChanged",
        "connectionError",
        "error",
        "onNetworkMoveReceived",
        "fromRow",
        "fromCol",
        "toRow",
        "toCol",
        "onConnectionEstablished",
        "onConnectionError",
        "onBoardUpdated",
        "onNetworkGameEnded",
        "reason",
        "startNewGame",
        "mode",
        "playerColor",
        "makeMove",
        "getBoardState",
        "QVariantList",
        "getMoveHistory",
        "saveGame",
        "filename",
        "selectPiece",
        "row",
        "col",
        "clearSelection",
        "startServer",
        "port",
        "connectToServer",
        "ip",
        "currentPlayer",
        "Color",
        "gameState",
        "GameState",
        "moveHistory",
        "isNetworkGame",
        "isServer",
        "canMakeMove",
        "isConnected",
        "isConnecting",
        "myColor"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'currentPlayerChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameStateChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messageShown'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'boardUpdated'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'historyChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'networkGameChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'connectionStatusChanged'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'canMakeMoveChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'myColorChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isConnectingChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'connectionError'
        QtMocHelpers::SignalData<void(const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'onNetworkMoveReceived'
        QtMocHelpers::SlotData<void(int, int, int, int)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 17 }, { QMetaType::Int, 18 }, { QMetaType::Int, 19 }, { QMetaType::Int, 20 },
        }}),
        // Slot 'onConnectionEstablished'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onConnectionError'
        QtMocHelpers::SlotData<void(const QString &)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'onBoardUpdated'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNetworkGameEnded'
        QtMocHelpers::SlotData<void(const QString &)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 25 },
        }}),
        // Method 'startNewGame'
        QtMocHelpers::MethodData<void(int, int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 },
        }}),
        // Method 'makeMove'
        QtMocHelpers::MethodData<void(int, int, int, int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 17 }, { QMetaType::Int, 18 }, { QMetaType::Int, 19 }, { QMetaType::Int, 20 },
        }}),
        // Method 'getBoardState'
        QtMocHelpers::MethodData<QVariantList() const>(30, 2, QMC::AccessPublic, 0x80000000 | 31),
        // Method 'getMoveHistory'
        QtMocHelpers::MethodData<QVariantList() const>(32, 2, QMC::AccessPublic, 0x80000000 | 31),
        // Method 'saveGame'
        QtMocHelpers::MethodData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Method 'selectPiece'
        QtMocHelpers::MethodData<void(int, int)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 36 }, { QMetaType::Int, 37 },
        }}),
        // Method 'clearSelection'
        QtMocHelpers::MethodData<void()>(38, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'startServer'
        QtMocHelpers::MethodData<void(int)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 40 },
        }}),
        // Method 'connectToServer'
        QtMocHelpers::MethodData<void(const QString &, int)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 42 }, { QMetaType::Int, 40 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'currentPlayer'
        QtMocHelpers::PropertyData<Color>(43, 0x80000000 | 44, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'gameState'
        QtMocHelpers::PropertyData<GameState>(45, 0x80000000 | 46, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 1),
        // property 'moveHistory'
        QtMocHelpers::PropertyData<QVariantList>(47, 0x80000000 | 31, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 4),
        // property 'isNetworkGame'
        QtMocHelpers::PropertyData<bool>(48, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'isServer'
        QtMocHelpers::PropertyData<bool>(49, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'canMakeMove'
        QtMocHelpers::PropertyData<bool>(50, QMetaType::Bool, QMC::DefaultPropertyFlags, 7),
        // property 'isConnected'
        QtMocHelpers::PropertyData<bool>(51, QMetaType::Bool, QMC::DefaultPropertyFlags, 6),
        // property 'isConnecting'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags, 9),
        // property 'myColor'
        QtMocHelpers::PropertyData<Color>(53, 0x80000000 | 44, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 8),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameWindow, qt_meta_tag_ZN10GameWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10GameWindowE_t>.metaTypes,
    nullptr
} };

void GameWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->currentPlayerChanged(); break;
        case 1: _t->gameStateChanged(); break;
        case 2: _t->messageShown((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->boardUpdated(); break;
        case 4: _t->historyChanged(); break;
        case 5: _t->networkGameChanged(); break;
        case 6: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->canMakeMoveChanged(); break;
        case 8: _t->myColorChanged(); break;
        case 9: _t->isConnectingChanged(); break;
        case 10: _t->connectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->onNetworkMoveReceived((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 12: _t->onConnectionEstablished(); break;
        case 13: _t->onConnectionError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->onBoardUpdated(); break;
        case 15: _t->onNetworkGameEnded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->startNewGame((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 17: _t->makeMove((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 18: { QVariantList _r = _t->getBoardState();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 19: { QVariantList _r = _t->getMoveHistory();
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->saveGame((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 21: _t->selectPiece((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 22: _t->clearSelection(); break;
        case 23: _t->startServer((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->connectToServer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::currentPlayerChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::gameStateChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)(const QString & )>(_a, &GameWindow::messageShown, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::boardUpdated, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::historyChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::networkGameChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)(const QString & )>(_a, &GameWindow::connectionStatusChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::canMakeMoveChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::myColorChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)()>(_a, &GameWindow::isConnectingChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameWindow::*)(const QString & )>(_a, &GameWindow::connectionError, 10))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<Color*>(_v) = _t->getCurrentPlayer(); break;
        case 1: *reinterpret_cast<GameState*>(_v) = _t->getGameState(); break;
        case 2: *reinterpret_cast<QVariantList*>(_v) = _t->getMoveHistory(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isNetworkGame(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->isServer(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->canMakeMove(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->isConnected(); break;
        case 7: *reinterpret_cast<bool*>(_v) = _t->isConnecting(); break;
        case 8: *reinterpret_cast<Color*>(_v) = _t->getMyColor(); break;
        default: break;
        }
    }
}

const QMetaObject *GameWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10GameWindowE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 25;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void GameWindow::currentPlayerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GameWindow::gameStateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GameWindow::messageShown(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void GameWindow::boardUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void GameWindow::historyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void GameWindow::networkGameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void GameWindow::connectionStatusChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void GameWindow::canMakeMoveChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void GameWindow::myColorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void GameWindow::isConnectingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void GameWindow::connectionError(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}
QT_WARNING_POP
