QT -= core gui
#QT += xlsx
CONFIG += c++17 c++1z
CONFIG -= warn_off warn_on

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -Wno-deprecated -Wno-write-strings -Wno-unused-result -Wno-format-security
QMAKE_CXXFLAGS += -static -static-libgcc -static-libstdc++
QMAKE_CXXFLAGS += -O0
QMAKE_CXXFLAGS += -g -ggdb -pg -O0
#QMAKE_CXXFLAGS += -pg -O0
QMAKE_CFLAGS += -static -static-libgcc
DEFINES += QT_NO_KEYWORDS

linux-g++* {
	DEFINES += MANA_WT
	LIBS += -L/dalaran/mana/out/ -lmana
	PRE_TARGETDEPS += /dalaran/mana/out/libmana.a
	LIBS += -lcurl -lssh
	LIBS += -lwthttp -lwt -lwtdbo -lwtdbomysql
}

INCLUDEPATH += /dalaran/mana/mana
DEPENDPATH += /dalaran/mana/mana

TARGET = brinespell
TEMPLATE = app
DESTDIR     = ../out/
OBJECTS_DIR = ../out/
MOC_DIR = ../out/

HEADERS  += config.h\
				test/unit_test.h\
				db/dbo_version.h\
				db/user.h\
				db/player.h\
				db/campaign.h\
				broadcast.h\
				soma.h\
				soma_database.h\
				widget/wcontainer_nolayout.h\
				widget/wcontainer.h\
				widget/widget_login.h\
				widget/widget_search.h\
				widget/widget_audio.h\
				widget/widget_dice.h\
				widget/widget_chat.h\
				widget/widget_image.h\
				widget/widget_home.h\
				thread/background.h\

SOURCES +=	main.cpp\
				config.cpp\
				test/unit_test.cpp\
				db/user.cpp\
				db/player.cpp\
				db/campaign.cpp\
				broadcast.cpp\
				soma.cpp\
				soma_database.cpp\
				widget/wcontainer_nolayout.cpp\
				widget/wcontainer.cpp\
				widget/widget_login.cpp\
				widget/widget_search.cpp\
				widget/widget_audio.cpp\
				widget/widget_dice.cpp\
				widget/widget_chat.cpp\
				widget/widget_image.cpp\
				widget/widget_home.cpp\
				thread/background.cpp\
