# filter_sec

#'@title Filtering secondary tags
#'@name filter_sec
#'@description
#'This function provides a handy way of filtering a cppRnet data set for specific secondary tags.
#'It will return a data set to which will be added new columns based on the specific secondary keys and values that 
#'are looked for.
#'
#'@param data a cppRnet data set, with an `attrs` column.
#'@param keys either a vector of valid key parameters, or a named list where names correspond to keys and 
#'attributes are vectors of matching values.
#'@param cores number of cores to parallelise the computation
#'
#'@examples
#'
#'test_file <- system.file(package = 'cppRnet','extdata','map.osm')
#'data <- cppRnet::extract_data(test_file)
#'
#'
#'keys <- list("cuisine"=c("pizza","japanese")
#'             ,"takeaway"=c("yes"))
#'
#'data_filtered <- cppRnet::filter_sec(data,keys)
#'
#'@export
filter_sec <- function(data,keys,cores=1){
  
  if(!is.null(names(keys))) {
    
    cli::cli_alert_info("Searching for exact key~value matches.")
    
    keys_found <- parallel::mcmapply(data$attrs
                                     ,mc.cores = cores
                                     ,FUN=\(attr){
                                      any(mapply(attr[names(keys)],keys,FUN=\(x,y) any(grepl(pattern=y,x=x))))
                                     })
    
    data[keys_found,attrs]
    
    extra_cols <- parallel::mcmapply(data[keys_found,attrs]
                                     ,mc.cores=cores
                                     ,SIMPLIFY=FALSE
                                     ,FUN=\(matches){
                                       matches[names(keys)] |> 
                                         # `names<-`() |>
                                         data.table::as.data.table()
                                     }) |> 
      data.table::rbindlist(use.names=TRUE,fill=TRUE)
    
    return(cbind(data[keys_found,],extra_cols))
  }
  
  keys_found <- parallel::mcmapply(data$attrs
                                   ,mc.cores = cores
                                   ,FUN=\(attr){
                                     if(any(keys%in%names(attr))) TRUE
                                     else FALSE
                                   })
  
  if(length(which(keys_found))==0) cli::cli_alert_warning("No matching keys found.")
  
  extra_cols <- parallel::mcmapply(data[keys_found,attrs]
                    ,mc.cores=cores
                    ,SIMPLIFY=FALSE
                    ,FUN=\(matches){
                      matches[keys] |> 
                        `names<-`(keys) |> 
                        data.table::as.data.table()
                    }) |> 
    data.table::rbindlist(use.names=TRUE,fill=TRUE)
  
  if(length(extra_cols)<length(keys)) cli::cli_alert_warning("Some keys are missing in the data.")
  
  return(cbind(data[keys_found,],extra_cols))
}


