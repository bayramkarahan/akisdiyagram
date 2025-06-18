QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    =   mainwindow.h \
		diagramitem.h \
		diagramscene.h \
		arrow.h \
		diagramtextitem.h \
    diagram.h \
    dotsignal.h \
    variable.h \
    variable_selection_dialog.h \
    variableconditiondialog.h \
    variableeditform.h \
    variableeditordialog.h \
    variableexpressiondialog.h \
    variableoutputdialog.h
SOURCES	    =   mainwindow.cpp \
		diagramitem.cpp \
		main.cpp \
		arrow.cpp \
		diagramtextitem.cpp \
		diagramscene.cpp \
    dotsignal.cpp \
    variable.cpp \
    variable_selection_dialog.cpp \
    variableconditiondialog.cpp \
    variableeditform.cpp \
    variableeditordialog.cpp \
    variableexpressiondialog.cpp \
    variableoutputdialog.cpp
RESOURCES   =	diagramscene.qrc


# install
target.path = /usr/bin
desktop_file.files = akisdiyagram.desktop
desktop_file.path = /usr/share/applications/
icon.files = images/prg.png
icon.path = /usr/share/akisdiyagram

#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target desktop_file icon

DISTFILES += \
    images/prg.png
