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
  
  # graph_data <- cpp_extract_graph(filename)
  # graph_data <- lapply(graph_data,data.table::as.data.table)
  # graph_data$nodes <- graph_data$nodes[!duplicated(graph_data$nodes$id),]
  # return(graph_data)
  
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
extract_data <- function(filename, main_keys = NULL){
  
  assert_tool_is_installed('osmium')
  
  main_first_level <- c(
    # "aerialway" 
    # ,"aeroway"
    "amenity"
    # ,"barrier"
    # ,"boundary"
    # ,"building" # is too generic
    ,"craft"
    # ,"emergency" # inconsistent data here
    # ,"geological" # too scarce
    ,"healthcare" # more consistent compared to emergency
    # ,"highway" this is treated by a separate function
    ,"historic"
    # ,"landuse" # generic as well
    ,"leisure" # overlaps with sports
    # ,"military" # scarce
    # ,"natural" # 
    ,"office" # a bit of mix between amenity, shop
    ,"power" # 
    # ,"railway" # add option to export with highways
    ,"shop"
    # ,"telecom" # scarce
    ,"tourism"
    # ,"waterway" # very specific
  )
  
  if(is.null(main_keys)){
    
    cli::cli_alert('Using main keys `amenity`,`shop`,`tourism`.')
    
    data <- cpp_extract_data(filename,main_sel=c("amenity","shop","tourism"))  
  
  } else {
    stopifnot(all(main_keys %in% main_first_level))
    
  }
  
  if (all(sapply(data,length)==length(data[[1]])
          ,length(data[[1]]>0))){
    return(data.table::as.data.table(data))
  }
  cli::cli_alert_danger('Could not extract consistent data, please check source data.')
}
