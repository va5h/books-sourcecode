import arcpy

sgdb_path = "c:/gdb"
sgdb_name = "my_python_gdb.gdb"

arcpy.CreateFileGDB_management(sgdb_path, sgdb_name)
input ("File created successfully, press any key to continue...")
