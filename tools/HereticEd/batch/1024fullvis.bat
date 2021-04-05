@echo Full Vis
@echo .
qbsp3 -chop 1024 %1
qvis3 %1
qrad3 %1
