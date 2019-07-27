#-------------------------------------------------
#
# Project created by QtCreator 2018-12-27T13:11:19
#
#-------------------------------------------------

QT       += core gui
QT       += concurrent

            greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

                        TARGET = inz2
                                 TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
                                            DEFINES += QT_DEPRECATED_WARNINGS
                                                    CONFIG += c++17
                                                            QMAKE_CXXFLAGS += -std=c++17# -Wall -Wextra -Werror -pedantic -Wshadow

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

                                                                    INCLUDEPATH += /usr/src/googletest/googletest/ /usr/src/googletest/googletest/include /usr/src/googletest/googletest/include/gtest
                                                                            INCLUDEPATH += /usr/src/googletest/googlemock /usr/src/googletest/googlemock/include /usr/src/googletest/googlemock/include/gmock

                                                                                    SOURCES += \
                                                                                            main.cpp \
                                                                                            Controller/pdfreader.cpp \
                                                                                            /usr/src/googletest/googletest/src/gtest-all.cc \
                                                                                            /usr/src/googletest/googlemock/src/gmock-all.cc \
                                                                                            View/window.cpp \
                                                                                            Controller/plagiarismchecker.cpp

                                                                                            HEADERS += \
                                                                                                    Controller/pdfreader.h \
                                                                                                    View/window.h \
                                                                                                    Controller/plagiarismchecker.h

                                                                                                    FORMS +=

                                                                                                            LIBS += -lgtest -L/usr/local/lib/libgtest.a
                                                                                                                    LIBS += -lgmock -L/usr/local/lib/libgmock.a
