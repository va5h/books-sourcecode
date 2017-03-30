require([
  "dojo/parser",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/on",
  "esri/map",
  "y2k/bootstrapmap",
  "y2k/Census",
  "bootstrap/Dropdown",
  "bootstrap/Modal",
  "dojo/domReady!"
], function(
  parser, lang, dom, dojoOn, Map, BootstrapMap, Census
) {
  parser.parse();
  
  var map = BootstrapMap.create("map", {
    basemap: "national-geographic",
    center: [-95, 45],
    zoom: 3
  });
  
  var census = new Census({
    map: map,
    mapService: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/"
  }, "census-widget");
  
  dojoOn(dom.byId("census-btn"), "click", lang.hitch(census, census.show));
  
});