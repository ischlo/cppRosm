#'@title Extracting road graphs from OSM
#'@name extract_graph
#'@description
#'This function helps extract a road network graph in a simple and light weight format.
#'In the background, it uses the power of the osmium library in C++, and copies the data iin graph format locally as 2 .csv 
#'files, one with nodes and their coordinates, with with segments and optional values. 
#'
#'@param filename a local osm extract file with .osm or .osm.pbf extension.
#'
#' local_osm_filename <- system.file("extdata",'map.osm', package = "cppRnet")
#'
#' cppRnet::extract_graph(local_osm_filename)
#'
#'@export
extract_graph <- function(filename){
  
  assert_tool_is_installed('osmium')
  
  f_names <- c('nodes.csv','road_segments.csv')
  
  if(any(sapply(f_names,file.exists))) stop('Filenames `nodes.csv` or `road_segments.csv` exist, please rename or remove.')
  
  cat('Files will be written to: ',getwd(),'\n')
  
  cpp_extract_graph(filename)
}




#'@title Extracting data from raw osm.
#'@name extract_data
#'@description
#'
#'Getting non-network data from raw osm files.
#'
#'@param filename a local osm extract file with .osm or .osm.pbf extension.
#'
#' local_osm_filename <- system.file("extdata",'map.osm', package = "cppRnet")
#'
#' data <- cppRnet::extract_data(local_osm_filename)
#' 
#' summary(data)
#'
#'
#'@export
extract_data <- function(filename){
  
  assert_tool_is_installed('osmium')
  
  
  
  data <- cpp_extract_data(filename)
  
  if (all(sapply(data,length)==length(data[[1]])
          ,length(data[[1]]>0))){
    return(data.table::as.data.table(data))
  }
  cli::cli_alert_danger('Could not extract consistent data, please check source data.')
}
