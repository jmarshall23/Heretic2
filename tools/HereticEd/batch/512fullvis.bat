@echo Full Vis
@echo .
qbsp3 -chop 512 %1
qvis3 %1
qrad3 %1
