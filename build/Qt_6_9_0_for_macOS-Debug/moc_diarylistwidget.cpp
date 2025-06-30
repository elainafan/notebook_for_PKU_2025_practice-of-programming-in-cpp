/****************************************************************************
** Meta object code from reading C++ file 'diarylistwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../diarylistwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'diarylistwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14DiaryListEntryE_t {};
} // unnamed namespace

template <> constexpr inline auto DiaryListEntry::qt_create_metaobjectdata<qt_meta_tag_ZN14DiaryListEntryE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DiaryListEntry",
        "checkUpdated",
        "",
        "beingPressed",
        "requestDelete",
        "requestShowAll"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'checkUpdated'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'beingPressed'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestDelete'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestShowAll'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DiaryListEntry, qt_meta_tag_ZN14DiaryListEntryE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DiaryListEntry::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DiaryListEntryE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DiaryListEntryE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14DiaryListEntryE_t>.metaTypes,
    nullptr
} };

void DiaryListEntry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DiaryListEntry *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->checkUpdated(); break;
        case 1: _t->beingPressed(); break;
        case 2: _t->requestDelete(); break;
        case 3: _t->requestShowAll(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DiaryListEntry::*)()>(_a, &DiaryListEntry::checkUpdated, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListEntry::*)()>(_a, &DiaryListEntry::beingPressed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListEntry::*)()>(_a, &DiaryListEntry::requestDelete, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListEntry::*)()>(_a, &DiaryListEntry::requestShowAll, 3))
            return;
    }
}

const QMetaObject *DiaryListEntry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiaryListEntry::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14DiaryListEntryE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DiaryListEntry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DiaryListEntry::checkUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DiaryListEntry::beingPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DiaryListEntry::requestDelete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DiaryListEntry::requestShowAll()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
namespace {
struct qt_meta_tag_ZN15DiaryListWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto DiaryListWidget::qt_create_metaobjectdata<qt_meta_tag_ZN15DiaryListWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DiaryListWidget",
        "changeList",
        "",
        "num",
        "newListAdded",
        "deleteList",
        "showAll",
        "hideScrollBar",
        "updateBtnState",
        "addNewList",
        "DiaryList"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'changeList'
        QtMocHelpers::SignalData<void(const int &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Signal 'newListAdded'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'deleteList'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'showAll'
        QtMocHelpers::SignalData<void(const int &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'hideScrollBar'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateBtnState'
        QtMocHelpers::SlotData<void(const int &)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 3 },
        }}),
        // Slot 'addNewList'
        QtMocHelpers::SlotData<void(const DiaryList &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 2 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DiaryListWidget, qt_meta_tag_ZN15DiaryListWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DiaryListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DiaryListWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DiaryListWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15DiaryListWidgetE_t>.metaTypes,
    nullptr
} };

void DiaryListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DiaryListWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->changeList((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->newListAdded(); break;
        case 2: _t->deleteList(); break;
        case 3: _t->showAll((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->hideScrollBar(); break;
        case 5: _t->updateBtnState((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->addNewList((*reinterpret_cast< std::add_pointer_t<DiaryList>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DiaryListWidget::*)(const int & )>(_a, &DiaryListWidget::changeList, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListWidget::*)()>(_a, &DiaryListWidget::newListAdded, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListWidget::*)()>(_a, &DiaryListWidget::deleteList, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (DiaryListWidget::*)(const int & )>(_a, &DiaryListWidget::showAll, 3))
            return;
    }
}

const QMetaObject *DiaryListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiaryListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15DiaryListWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DiaryListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DiaryListWidget::changeList(const int & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void DiaryListWidget::newListAdded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DiaryListWidget::deleteList()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DiaryListWidget::showAll(const int & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
