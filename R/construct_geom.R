
#'@title construct_geom
#'@name construct_geom
#'@description
#'From a cppRnet data table, make the geometries.
#'
#'@param cpprnet_dt a data table obtained with cppRnet extract_data()
#'@param complete bool. TRUE to reconstruct Polygons, else use just points.
#'@param cores integer. Run the computation on multiple cores using `parallel`.
#'@examples
#'cpprnet_filepath <- system.file(package = 'cppRnet','extdata','map.osm')
#'cpprnet_dt <- cppRnet::extract_data(filename = cpprnet_filepath)
#'
#'cppRnet::construct_geom(cpprnet_dt,complete=TRUE)
#'
#'head(cpprnet_dt$geometry)
#'
#'@return Modifies the provided cpprnet_dt object and adds an `sf` geometry column to it, with either full geometries or just the centroids.
#' Full geometries typically contain polygons and points.
#'@export
construct_geom <- function(cpprnet_dt
                           ,complete = FALSE
                           ,cores = 1){
  stopifnot("attrs"%in% colnames(cpprnet_dt))

  if(complete){
    
    geometry <- rep_len(NA,nrow(cpprnet_dt))
    
    geom <- parallel::mclapply(cpprnet_dt[,attrs]
                               ,mc.cores = cores
                               ,FUN = \(attr){
                                 if(!is.null(attr$nodes)) {
                                   sf::st_polygon(x=list(as.matrix(attr[["nodes"]][,c("x","y")])))
                                 } else NA
                               })
    
    other_geom <- sapply(geom,is.na)
    
    geometry[!other_geom] <- geom[!other_geom]
    
    geometry[other_geom] <- cpprnet_dt[other_geom,list(lon,lat)] |> 
      sf::st_as_sf(coords=c("lon","lat"),crs=4326) |> 
      sf::st_geometry()
    
    return(cbind(cpprnet_dt,"geometry"=geometry |> sf::st_sfc(crs=4326)))
  } else {
    return(cbind(cpprnet_dt,"geometry"=sf::st_as_sf(cpprnet_dt[,.(lon,lat)],coords=c(1,2),crs=4326) |> sf::st_geometry()))
  }
}


