import arcpy

sfc_fullpath = "C:/gdb/my_python_gdb.gdb/my_python_featureclass"
sfield_name = "my_python_field"

arcpy.DeleteField_management(sfc_fullpath, sfield_name)

input ("Field has been deleted successfully, press any key to continue..")

