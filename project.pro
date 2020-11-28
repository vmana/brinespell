QT -= core gui
#QT += xlsx
CONFIG += c++17
CONFIG -= warn_off warn_on

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -Wno-deprecated -Wno-write-strings -Wno-unused-result -Wno-format-security
DEFINES += QT_NO_KEYWORDS

CONFIG(release, debug|release) {
	QMAKE_CXXFLAGS += -g -ggdb -O0
}
CONFIG(prod, prod) {
	QMAKE_CXXFLAGS += -g -ggdb -O3
}

linux-g++* {
	DEFINES += MANA_WT
	LIBS += -L/dalaran/mana/out/ -lmana
	PRE_TARGETDEPS += /dalaran/mana/out/libmana.a
	#LIBS += -lcurl -lssh
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
				db/inventory.h\
				db/notes.h\
				db/player.h\
				db/skill.h\
				db/campaign.h\
				db/npc.h\
				broadcast.h\
				soma.h\
				soma_database.h\
				widget/wcontainer.h\
				widget/widget_login.h\
				widget/widget_search.h\
				widget/widget_audio.h\
				widget/widget_dice.h\
				widget/widget_chat.h\
				widget/widget_image.h\
				widget/widget_initiative.h\
				widget/token/widget_token.h\
				widget/token/wtoken_player.h\
				widget/token/wtoken_npc.h\
				widget/character/widget_character.h\
				widget/character/widget_details_hp.h\
				widget/character/widget_stats.h\
				widget/character/widget_level.h\
				widget/character/widget_weapon.h\
				widget/character/widget_inventory.h\
				widget/character/widget_notes.h\
				widget/party/widget_party.h\
				widget/party/widget_ally.h\
				widget/widget_dynamic.h\
				widget/widget_home.h\
				thread/background.h\

SOURCES +=	main.cpp\
				config.cpp\
				test/unit_test.cpp\
				db/user.cpp\
				db/player.cpp\
				db/skill.cpp\
				db/campaign.cpp\
				db/npc.cpp\
				broadcast.cpp\
				soma.cpp\
				soma_database.cpp\
				widget/wcontainer.cpp\
				widget/widget_login.cpp\
				widget/widget_search.cpp\
				widget/widget_audio.cpp\
				widget/widget_dice.cpp\
				widget/widget_chat.cpp\
				widget/widget_initiative.cpp\
				widget/widget_image.cpp\
				widget/token/widget_token.cpp\
				widget/token/wtoken_player.cpp\
				widget/token/wtoken_npc.cpp\
				widget/character/widget_character.cpp\
				widget/character/widget_details_hp.cpp\
				widget/character/widget_stats.cpp\
				widget/character/widget_level.cpp\
				widget/character/widget_weapon.cpp\
				widget/character/widget_inventory.cpp\
				widget/character/widget_notes.cpp\
				widget/party/widget_party.cpp\
				widget/party/widget_ally.cpp\
				widget/widget_dynamic.cpp\
				widget/widget_home.cpp\
				thread/background.cpp\
