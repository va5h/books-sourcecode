import arcpy

sgdb_fullpath="c:/gdb/my_python_gdb.gdb"
sfc_name = "my_python_featureclass"
sgeometry = "POLYGON"
arcpy.CreateFeatureclass_management(sgdb_fullpath,sfc_name,sgeometry)
input ("Feature class created successfully, press any key to continue...")
