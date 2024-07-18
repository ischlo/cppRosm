# oop 
# creating the cppRnet data.frame/data.table S3 class for better consistency.

# new_myclass()
# validate_myclass()
# myclass()

# a cppRnet class object is also a data.frame, or data.table. 
# it has a few restrictions, mainly the attrs column that should contain named lists.
# an id column with osm ids.
# a lon and lat columns
# a key and value columns in the simplest form. 

# any method of the cppRnet package that uses cppRnet data tables should ideally do a validity check.

new_cppRnet_dt <- function(dt = data.frame()){
  
  stopifnot(all(c("attrs","id","lon","lat","key","value") %in% colnames(dt))
            ,as.data.frame(dt)
            )
  
  return(data.table::as.data.table(dt))
}



validate_cppRnet_dt <- function(dt=data.frame()){
  stopifnot(
    # these columns must be there 
    all(c("attrs","id","lon","lat","key","value") %in% colnames(dt))
    # should be a data table
    ,data.table::is.data.table(dt)
    # the `attrs` column contains lists
    ,is.list(dt[["attrs"]])
    # all the elements of `attrs` are named lists
    ,all(sapply(dt[["attrs"]],FUN = \(attr) (!is.null(names(attr)) | length(attr)==0)))
    # none of the following columns have NAs. the value column should probably be included.
    ,all(sapply(dt[,c("id","lon","lat","key")],FUN = \(x) all(!is.na(x))))
  )
  TRUE
}
