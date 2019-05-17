1. 學號：B04502139
2. 姓名：戴瑋辰
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU gcc 4.8.5>
5. 檔案壓縮方式：< tar -zcvf b04502139-pa3.tgz PA3>
6. 檔案說明：
     PA3/src/*h       : 所有的header檔
     PA3/src/*cpp     : 所有的cpp檔
     PA3/src/Makefile : linux makefile
     PA3/src/fp       : 執行檔

7. 編譯說明：
     進入PA3/src/
     輸入make clean && make
     makefile 將會產生執行檔 fp 於 ../bin/ 資料夾

8. 執行方式說明：
     執行黨的明令格式如下：
        ../bin/fp <alpha> <input block> <input net> <output>
        例如：要做 ami49 的 floor planning
        則輸入
        ../bin/fp 0.5 ami49.block ami49.net ami49.out
        此時最終結果會存放到 ami49.out 之中
