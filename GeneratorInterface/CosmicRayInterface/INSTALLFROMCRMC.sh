svn co https://devel-ik.fzk.de/svn/mc/crmc/trunk

mv trunk/src/*.inc*         src/
mv trunk/src/*.f            src/
mv trunk/src/*.F            src/
mv trunk/src/epos/*.f       src/
mv trunk/src/sibyll/*.f     src/
mv trunk/src/qgsjet/*.f     src/
mv trunk/src/qgsjetII-04/*.f   src/
mv trunk/src/lzma-read/*.*   src/
#mv trunk/src/crmc.param.in  crmc.param
mv trunk/tabs/              .

rm src/dummy.f
rm -rf trunk

sed -i 's%@CRMCROOT@%${PWD}%g' crmc.param #put in the tabs location
sed -i 's%Length  1.%Length  1.0%g' crmc.param #put it to decay 10^-12s
sed -i '588,627 s%^%c%' src/crmc-aaa.f #comment out random generator
sed -i '4943,4943 s%^%c%' src/epos-bas.f #comment out output seed if(nrevt.eq.0)
sed -i 's%nmxhep=9990%nmxhep=99900%g' src/crmc-aaa.f src/epos.inc #increase number of allowed particles
sed -i 's%hepevt%hepcom%g' src/epos.inc #this is a fix for a crash
echo " !!!disable boost!!! ycm2det=0"
