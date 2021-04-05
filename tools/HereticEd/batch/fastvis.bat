@echo Fast Vis
@echo .
qbsp3 %1
qvis3 -fast %1
qrad3 -bounce 0 %1
