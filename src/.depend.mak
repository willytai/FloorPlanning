Parser.o: Parser.cpp Mgr.h Block.h Net.h Terminal.h BStarTree.h myUsage.h
Mgr.o: Mgr.cpp Mgr.h Block.h Net.h Terminal.h BStarTree.h
myUsage.o: myUsage.cpp myUsage.h
BStarTree.o: BStarTree.cpp BStarTree.h Block.h RandNumGenerator.h
main.o: main.cpp myUsage.h Mgr.h Block.h Net.h Terminal.h BStarTree.h \
 RandNumGenerator.h
