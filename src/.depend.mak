Parser.o: Parser.cpp Mgr.h Block.h Net.h Terminal.h BStarTree.h myUsage.h
SA.o: SA.cpp Mgr.h Block.h Net.h Terminal.h BStarTree.h myUsage.h \
 RandNumGenerator.h
Mgr.o: Mgr.cpp Mgr.h Block.h Net.h Terminal.h BStarTree.h myUsage.h \
 RandNumGenerator.h
myUsage.o: myUsage.cpp myUsage.h
main.o: main.cpp myUsage.h Mgr.h Block.h Net.h Terminal.h BStarTree.h \
 RandNumGenerator.h
