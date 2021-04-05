@echo Fast Vis No Water
@echo .
qbsp3 -nowater %1
qvis3 -fast %1
qrad3 -bounce 0 %1
