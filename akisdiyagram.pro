QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    =   mainwindow.h \
		diagramitem.h \
		diagramscene.h \
		arrow.h \
		diagramtextitem.h \
    diagram.h \
    dotsignal.h
SOURCES	    =   mainwindow.cpp \
		diagramitem.cpp \
		main.cpp \
		arrow.cpp \
		diagramtextitem.cpp \
		diagramscene.cpp \
    dotsignal.cpp
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
