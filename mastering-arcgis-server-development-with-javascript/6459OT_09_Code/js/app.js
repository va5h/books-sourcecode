require([
  "dojox/mobile/parser",
  "dojo/dom",
  "dojo/dom-construct",
  //"dojo/dom-style",
  "dojo/on",
  "esri/config",
  "esri/map",
  "esri/graphic",
  "esri/layers/FeatureLayer",
  "esri/layers/ArcGISDynamicMapServiceLayer",
  "esri/symbols/SimpleMarkerSymbol",
  "esri/geometry/Extent",
  "esri/dijit/editing/Editor",
  "esri/dijit/editing/TemplatePicker",
  "esri/dijit/editing/AttachmentEditor",
  // "esri/dijit/editing/AttributeInspector",
  "esri/dijit/LocateButton",
  "esri/tasks/query",
  "dijit/registry",
  "dojox/mobile/Button",
  "dojox/mobile",
  "dojox/mobile/deviceTheme",
  "dojox/mobile/compat",
  "dojox/mobile/View",
  "dojox/mobile/Heading",
  "dojox/mobile/ToolBarButton",
  "dojox/mobile/SimpleDialog",
  "dojox/mobile/RoundRect",
  "dojox/mobile/TextBox",
  "dijit/form/DataList",
  "dojox/mobile/ComboBox",
  "dojo/domReady!"
], function (
  parser, dojoDom, domConstruct, dojoOn,
  esriConfig, Map, Graphic,
  FeatureLayer, ArcGISDynamicMapServiceLayer,
  MarkerSymbol, Extent,
  Editor, TemplatePicker, AttachmentEditor,
  // AttributeInspector,
  LocateButton, Query, registry, Button
) {

  // bounds for teh city and surrounding area
  var maxExtent = new Extent({
    "xmin":-13519092.335425414,
    "ymin":4413224.664902497,
    "xmax":-13507741.43672508,
    "ymax":4421766.502813354,
    "spatialReference":{"wkid":102100}
  }),
  map, incidentLayer, visibleIncidentLayer, selected, updateFeature, attInspector, picker;

  // parse HTML page for dojo elements
  parser.parse();

  // set up proxy for editing.
  esriConfig.defaults.io.proxyUrl = "./proxy/proxy.ashx";

  //create map
  map = new Map("map", {
    basemap: "osm",
    extent: maxExtent,
    autoResize: false
  });

  // feature layer for editing
  incidentLayer = new FeatureLayer("http://sampleserver3.arcgisonline.com/ArcGIS/rest/services/SanFrancisco/311Incidents/FeatureServer/0", {
    mode: FeatureLayer.MODE_SELECTION,
    outFields: ["req_type","req_date","req_time","address","district","status"],
    id: "incidentLayer"
  });

  // visible layer for existing incidents
  visibleIncidentLayer = new ArcGISDynamicMapServiceLayer("http://sampleserver3.arcgisonline.com/ArcGIS/rest/services/SanFrancisco/311Incidents/MapServer");

  // refresh the visible layer when the edit layer is updated.
  incidentLayer.on("edits-complete", function() {
    visibleIncidentLayer.refresh();
  });

  //
  function startEditing () {

    // add the Locate button
    var locator = new LocateButton({map: map}, "locatebutton");
    var incidentLayer = map.getLayer("incidentLayer");

    picker = new TemplatePicker({
      featureLayers: [ incidentLayer ],
      style: "width:100%;height:auto;",
      grouping: false
    }, "pickerdiv");
    picker.startup();

    attInspector = new AttachmentEditor({}, "attributediv");
    attInspector.startup();

    // add map click event to create the
    map.on("click", function(evt) {
      if (!maxExtent.contains(evt.mapPoint)) {
        alert("Sorry, that point is outside our area of interest.");
        return;
      }
      showInspector(evt);
    });

    incidentLayer.setSelectionSymbol(
      new MarkerSymbol({color:[255,0,0]})
    );

    incidentLayer.on("click", showInspector);

    //add a save button next to the delete button
    var saveButton = registry.byId("saveButton");

    saveButton.on("click", saveEdits);
  }

  function goToPicker(point) {
    registry.byId('mapview').performTransition('incidentview', 1, 'slide', function() {
      var picker = registry.byId('pickerdiv');

      //refresh the grid used by the templatePicker.
      picker.grid.render();
      picker.clearSelection();

      dojoOn.once(picker, 'selection-change', function () {

	var selected = picker.getSelected();
        if (selected) {
	    // log the date and time
	  var currentDate = new Date();
          var incidentAttributes = {
            req_type: selected.template.name,
	    req_date:(currentDate.getMonth() + 1) + "/" + currentDate.getDate() + "/" + currentDate.getFullYear(),
	    req_time: currentDate.toLocaleTimeString(),
	    address: "",
	    district: "",
            status: 1
          };
	  addIncident(point, selected.symbol, incidentAttributes)
	}
      });
    });
  }

  function addIncident(point, symbol, attributes) {
      var incidentGraphic = new Graphic(point, symbol, attributes);
      incidentLayer.applyEdits([incidentGraphic],null,null).then(function () {

	var selectQuery = new Query(),
  	    mapScale = map.getScale();
	    selectQuery.geometry = new Extent({
	      xmin: point.x - mapScale / 96,
	      xmax: point.x + mapScale / 96,
	      ymin: point.y - mapScale / 96,
	      ymax: point.y + mapScale / 96,
	      spatialReference: map.spatialReference
	    });

	incidentLayer.selectFeatures(selectQuery, FeatureLayer.SELECTION_NEW, function (features) {
	  if (features.length) {
	    // fill in the items
	    setupIncident(features[0], "incidentview");
	  }
	});
      });
  }

  function setupIncident(feature, view) {
    var attr = feature.attributes;
    updateFeature = feature;
    registry.byId("incidentSelector").set("value", attr.req_type);
    dojoDom.byId("incidentDate").innerHTML = attr.req_date;
    dojoDom.byId("incidentTime").innerHTML = attr.req_time;
    registry.byId("incidentAddress").set("value", attr.address);
    registry.byId("incidentDistrict").set("value", attr.district);
    dojoDom.byId("incidentStatus").innerHTML = attr.status;

    attInspector.showAttachments(feature, incidentLayer);
    registry.byId(view).performTransition('attributeview', 1, 'slide');
  }

  function saveEdits(){
    // add updated values
    updateFeature.attributes.req_type = registry.byId("incidentSelector").get("value");
    updateFeature.attributes.address = registry.byId("incidentAddress").get("value");
    updateFeature.attributes.district = registry.byId("incidentDistrict").get("value");

    updateFeature.getLayer().applyEdits(null, [updateFeature], null);
    registry.byId("attributeview").performTransition("mapview", -1, 'slide');
  }

  function showInspector(evt) {
    var selectQuery = new Query(),
  	point = evt.mapPoint,
  	mapScale = map.getScale();
    selectQuery.geometry = new Extent({
      xmin: point.x - mapScale / 96,
      xmax: point.x + mapScale / 96,
      ymin: point.y - mapScale / 96,
      ymax: point.y + mapScale / 96,
      spatialReference: map.spatialReference
    });

    incidentLayer.selectFeatures(selectQuery, FeatureLayer.SELECTION_NEW, function (features) {
      if (!features.length) {
        goToPicker(point);
      } else {
        setupIncident(features[0], "mapview");
      }
    });
  }

  function onMapExtentChange (response) {
    if (!response.extent.intersects(maxExtent)) {
      // show the popup
    }
  }

  dojoOn(dojoDom.byId("outsidemessage"), "click", function () {
  	map.setExtent(maxExtent);
  })

  // zoom in to our location if available.
  map.on("extent-change", onMapExtentChange);

  map.on("layers-add-result", startEditing);

  map.addLayers([visibleIncidentLayer, incidentLayer]);
});
