QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    uniform.cpp \
    quick_search.cpp \
    graph.cpp \
    topology_builder.cpp \
    unionfind.cpp \
    topology_builder_configurational.cpp \
    betweenness.cpp \
    percolation_kcore.cpp \
    percolation_betweenness.cpp \
    percolation_edge.cpp \
    percolation_degree.cpp \
    statistical_calculus.cpp \
    q_exponential.cpp \
    power_law.cpp \
    percolation_vertex.cpp \
    breadth_first_search.cpp

HEADERS += \
    uniform.h \
    quick_search.h \
    graph.h \
    percolation.h \
    topology_builder.h \
    unionfind.h \
    topology_builder_configurational.h \
    betweenness.h \
    percolation_kcore.h \
    percolation_betweenness.h \
    heap_desc.h \
    heap_asc.h \
    percolation_edge.h \
    percolation_degree.h \
    statistical_calculus.h \
    q_exponential.h \
    power_law.h \
    percolation_vertex.h \
    breadth_first_search.h \
    probability_distribution.h
