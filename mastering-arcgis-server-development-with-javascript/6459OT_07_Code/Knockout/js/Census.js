define([
  "dojo/_base/declare",
  "dojo/_base/array",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/dom-style",
  "dojo/dom-attr", // new
  "dojo/json",
  "dojo/on",
  "dijit/_WidgetBase",
  "dijit/_TemplatedMixin",
  "dijit/_OnDijitClickMixin",

  "dojo/text!./templates/Census.html",
  "dojo/text!./templates/StateCensus.html",
  "dojo/text!./templates/CountyCensus.html",
  "dojo/text!./templates/BlockGroupCensus.html",
  "dojo/text!./templates/BlockCensus.html",

  "esri/tasks/IdentifyParameters",
  "esri/tasks/IdentifyTask",
  "esri/InfoTemplate",
  "esri/symbols/SimpleMarkerSymbol",
  "esri/symbols/SimpleFillSymbol",
  "esri/request",
  "esri/tasks/query",
  "esri/tasks/QueryTask",
  "esri/graphicsUtils",
  "esri/config",
  "d3/d3",
  "knockout"
], function (
  declare, arrayUtils, lang,
  dom,
  domStyle, domAttr,
  JSON, dojoOn,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  IdentifyParameters, IdentifyTask,
  InfoTemplate, MarkerSymbol, FillSymbol,
  esriRequest, Query, QueryTask, graphicsUtils, esriConfig,
  d3, ko
) {

  return declare([_WidgetBase, _TemplatedMixin, _OnDijitClickMixin], {

    baseClass: "y2k-census",
    declaredClass: "y2k.Census",
    templateString: dijitTemplate,
    constructor: function (options, srcRefNode) {

      if (typeof srcRefNode === "string") {
	srcRefNode = dom.byId(srcRefNode);
      }

      this.identifyTask = new IdentifyTask(options.mapService);
      this.map = options.map || null;
      this.domNode = srcRefNode;

      if (this.map.loaded) {
	this._onMapLoad();
      } else {
	this.map.on("load", lang.hitch(this, this._onMapLoad));
      }
    },

    SelectorViewModel: function () {
      this.stateList = ko.observableArray([]);
      this.selectedState = ko.observable();
      this.countyList = ko.observableArray([]);
      this.selectedCounty = ko.observable();
      this.blockGroupList = ko.observableArray([]);
      this.selectedBlockGroup = ko.observable();
    },

    postCreate: function () {

      var self = this;

      ko.bindingHandlers.stateUpdate = {
	update: function (el, valueAccessor, allBindings, vm, bindingContext) {
	  var stateData = ko.unwrap(valueAccessor()),
	    stateName, stateFIPS;

	  if (stateData && stateData.STATE_NAME) {
	    stateName = stateData.STATE_NAME || "";
	    stateFIPS = stateData.STATE_FIPS || "";
	    console.log("State Name: ", stateName);
	    console.log("State FIPSS: ", stateFIPS);

	    self.queryShapeAndData({
              url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/3",
              where: "STATE_NAME = '" + stateName + "'"
            }).addCallback(lang.hitch(self, self._onQueryComplete, "states")).then(
              lang.hitch(self, self._updateInfoWindowFromQuery),
              function (err) {
                console.log(err);
              });

	    // search for counties in that state.
	    self.queryDropdownData({
	      url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/2",
	      fields: ["NAME", "STATE_NAME", "CNTY_FIPS", "STATE_FIPS"],
	      where: "STATE_NAME = '" + stateName + "'",
	    }).then(function (featureSet) {
	      var theCounties = arrayUtils.map(featureSet.features, function (feature) {return feature.attributes;})
	      // insert a blank value in the front.
	      theCounties.splice(0, 0, {"NAME": "", "STATE_NAME": "", "CNTY_FIPS": "", "STATE_FIPS": ""});
	      bindingContext.$data.countyList(theCounties);
	    });
	  }
	}
      };

      ko.bindingHandlers.countyUpdate = {
	update: function (el, valueAccessor, allBindings, vm, bindingContext) {
	  var countyData = ko.unwrap(valueAccessor()),
	    stateName, countyName, stateFIPS, countyFIPS;

	  if (countyData && countyData.NAME) {
	    stateName = countyData.STATE_NAME
	    countyName = countyData.NAME || "";
	    stateFIPS = countyData.STATE_FIPS || "";
	    countyFIPS = countyData.CNTY_FIPS || "";

	    self.queryShapeAndData({
              url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/2",
              where: "STATE_NAME = '" + stateName + "' AND NAME = '" + countyName + "'"
            }).addCallback(lang.hitch(self, self._onQueryComplete, "Detailed Counties")).then(
              lang.hitch(self, self._updateInfoWindowFromQuery),
              function (err) {
                console.log(err);
              });

	    // search for block groups in that county.
	    self.queryDropdownData({
	      url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/1",
	      fields: ["BLKGRP", "CNTY_FIPS", "STATE_FIPS"],
	      where: "STATE_FIPS = '" + stateFIPS + "' AND CNTY_FIPS = '" + countyFIPS + "'",
	    }).then(function (featureSet) {
	      var theBlockGroups = arrayUtils.map(featureSet.features, function (feature) {return feature.attributes;})
	      // insert a blank value in the front.
	      theBlockGroups.splice(0, 0, {"BLKGRP": "", "CNTY_FIPS": "", "STATE_FIPS": ""});
	      bindingContext.$data.blockGroupList(theBlockGroups);
	    });

	  }
	}
      };

      ko.bindingHandlers.blockGroupUpdate = {
	update: function (el, valueAccessor, allBindings, vm, bindingContext) {
	  var blockGroupData = ko.unwrap(valueAccessor()),
	    blockGroup, stateFIPS, countyFIPS;

	  if (blockGroupData && blockGroupData.BLKGRP) {
	    blockGroup = blockGroupData.BLKGRP || "";
	    stateFIPS = blockGroupData.STATE_FIPS || "";
	    countyFIPS = blockGroupData.CNTY_FIPS || "";

	    self.queryShapeAndData({
              url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/1",
              where: "STATE_FIPS = '" + stateFIPS + "' AND CNTY_FIPS = '" + countyFIPS + "' AND BLKGRP = '" + blockGroup + "'"
            }).addCallback(lang.hitch(self, self._onQueryComplete, "Census Block Group")).then(
              lang.hitch(self, self._updateInfoWindowFromQuery),
              function (err) {
                console.log(err);
              });
	  }
	}
      };

      var vm = new this.SelectorViewModel();
      ko.applyBindings(vm);

      this.queryDropdownData({
	url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/3",
	fields: ["STATE_NAME", "STATE_FIPS"],
	where: "1=1",
      }).then(lang.hitch(this, function (featureSet) {
        var theStates = arrayUtils.map(featureSet.features, function (feature) { return feature.attributes; });
	theStates.splice(0, 0, {STATE_NAME: "", STATE_FIPS: ""});
        vm.stateList(theStates);

      }));
    },

    _onMapLoad: function () {
      // change the infoWindow symbol
      this.map.infoWindow.markerSymbol = new MarkerSymbol({
        style: "esriSMSDiamond",
        color: [255,200,30],
        size: 12,
        outline: {
          width: 1,
          color: [111,98,34]
        }
      });

      this.map.infoWindow.fillSymbol = new FillSymbol({
        style: "esriSFSSolid",
        color: [255,250,169,128],
        outline: {
          width: 2,
          color: [111,98,34]
        }
      });

      dojoOn(this.map.infoWindow, "selection-change", lang.hitch(this, this._onInfoWindowSelect));

    },

    show: function () {
      domStyle.set(this.domNode, "display", "block");
      this._mapClickHandler = this.map.on("click", lang.hitch(this, this._onMapClick));
    },

    hide: function () {
      domStyle.set(this.domNode, "display", "none");
      if (this._mapClickHandler && typeof this._mapClickHandler.remove === "function") {
        this._mapClickHandler.remove();
      }
    },

    _onMapClick: function (event) {
      var params = new IdentifyParameters(),
        defResults;

      params.geometry = event.mapPoint;
      params.layerOption = IdentifyParameters.LAYER_OPTION_ALL;
      params.mapExtent = this.map.extent;
      params.returnGeometry = true;
      params.width = this.map.width;
      params.height= this.map.height;
      params.spatialReference = this.map.spatialReference;
      params.tolerance = 3;

      this.map.graphics.clear();
      defResults = this.identifyTask.execute(params).addCallback(lang.hitch(this, this._onIdentifyComplete));
      this.map.infoWindow.setFeatures([defResults]);
      this.map.infoWindow.show(event.mapPoint);
    },

    _onIdentifyComplete: function (results) {

      return arrayUtils.map(results, lang.hitch(this, function (result) {
        return this._processFeature(result.feature, result.layerName);
      }));
    },

    _processFeature: function (feature, title) {
      var content;

      switch(title) {
        case "Census Block Points":
          title += " ${STATE_FIPS}";
          content = BlockTemplate;
          break;
        case "Census Block Group":
          title += " ${BLKGRP}";
          content = BlockGroupTemplate;
          break;
        case "Detailed Counties":
          title = "${NAME}";
          content = CountyTemplate;
          break;
        case "states":
          title = "${STATE_NAME}";
          content = StateTemplate;
          break;
        default:
          content = "${*}";
      }

      feature.infoTemplate = new InfoTemplate(title, content);

      return feature;
    },

    _updateInfoWindowFromQuery: function (results) {
      var resultExtent = graphicsUtils.graphicsExtent(results.features);

      this.map.setExtent(resultExtent).then(lang.hitch(this, function () {
	this.map.infoWindow.setFeatures(results.features);
	this.map.infoWindow.show(resultExtent.getCenter());
      }));
    },

    _onQueryComplete: function (title, featureSet) {
      featureSet.features = arrayUtils.map(featureSet.features, lang.hitch(this, function (feature) {
	return this._processFeature(feature, title);
      }));
    },

    // @param (object) args
    // @param (string) args.url- Map Service url for a QueryTask
    // @parma (string) args.where- Where clause to pass for the Query
    queryShapeAndData: function (args) {
      var queryTask = new QueryTask(args.url),
	query = new Query();
      query.returnGeometry = true;
      query.outFields = ["*"];
      query.outSpatialReference = this.map.spatialReference;
      query.where = args.where;

      return queryTask.execute(query);
    },

    // @param (object) args
    // @param (string) args.url- Map Service url for a QueryTask
    // @parma (string) args.where- Where clause to pass for the Query
    // @param (string[]) args.fields - fields to return and parse.
    queryDropdownData: function (args) {
      var queryTask = new QueryTask(args.url),
	query = new Query();
      query.returnGeometry = false;
      query.outFields = args.fields;
      query.orderByFields = args.fields;
      query.where = args.where;
      query.returnDistinctValues = true;

      return queryTask.execute(query);
    },


    _onInfoWindowSelect: function () {
      var graphic = this.map.infoWindow.getSelectedFeature(),
        ethnicData, genderData, ageData;
      if (graphic && graphic.attributes) {
        // load and render the ethnic data
        ethnicData = this.ethnicData(graphic.attributes);
        this.ethnicGraph(ethnicData);
        // load and render the gender data
        genderData = this.genderData(graphic.attributes);
        this.genderGraph(genderData);
        // load and render the age data
        ageData = this.ageData(graphic.attributes);
        this.ageGraph(ageData);
      }
    },


    formatAttributesForGraph: function (attributes, fieldLabels) {
      var data = [],
        field;
      for (field in fieldLabels) {
        data.push({
          name: fieldLabels[field],
          population: +attributes[field]
        })
      }
      return data;
    },

    ethnicData: function (attributes) {
      var fieldLabels = {
        "WHITE": "Caucasian",
        "BLACK": "African-American",
        "AMERI_ES": "Native American /<br> Alaskan Native",
        "ASIAN": "Asian",
        "HAWN_PI": "Hawaiian /<br> Pacific Islander",
        "HISPANIC": "Hispanic",
        "OTHER": "Other",
        "MULT_RACE": "Multiracial"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    ethnicGraph: function (data) {
      var width = 240,
        height = 210,
        radius = Math.min(width, height) / 2;

      var color = d3.scale.ordinal()
        .range(["#98abc5", "#8a89a6", "#7b6888", "#6b486b", "#a05d56", "#d0743c", "#ff8c00", "#c7d223"]);

      var arc = d3.svg.arc()
        .outerRadius(radius - 10)
        .innerRadius(0);

      var pie = d3.layout.pie()
        .sort(null)
        .value(function(d) { return d.population; });

      var svg = d3.select(".censusethnic").append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");


      if (!data || !data.length) {
        return;
      }

      var g = svg.selectAll(".arc")
        .data(pie(data))
        .enter().append("g")
        .attr("class", "arc");

      g.append("path")
        .attr("d", arc)
        .style("fill", function(d) { return color(d.data.name); });


      g.append("text")
        .attr("transform", function(d) { return "translate(" + arc.centroid(d) + ")"; })
        .attr("dy", ".35em")
        .style("text-anchor", "middle")
        .text(function(d) { return d.data.name; });

    },

    genderData: function (attributes) {
      var fieldLabels = {
        "MALES": "Males",
        "FEMALES": "Females"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    genderGraph: function (data) {
      var width = 240,
        height = 210,
        radius = Math.min(width, height) / 2;

      var color = d3.scale.ordinal()
        .range(["#8888ff", "#ff8888"]);

      var arc = d3.svg.arc()
        .outerRadius(radius - 10)
        .innerRadius(0);

      var pie = d3.layout.pie()
        .sort(null)
        .value(function(d) { return d.population; });

      var svg = d3.select(".censusgender").append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");

      if (!data || !data.length) {
        return;
      }

      var g = svg.selectAll(".arc")
        .data(pie(data))
        .enter().append("g")
        .attr("class", "arc");

      g.append("path")
        .attr("d", arc)
        .style("fill", function(d) { return color(d.data.name); });


      g.append("text")
        .attr("transform", function(d) { return "translate(" + arc.centroid(d) + ")"; })
        .attr("dy", ".35em")
        .style("text-anchor", "middle")
        .text(function(d) { return d.data.name; })
        .append("tspan")
        .text(function(d) { return d.data.population;})
        .attr("x", "0").attr("dy", '15');
    },

    ageData: function (attributes) {
      var fieldLabels = {
        "AGE_UNDER5": "< 5",
        "AGE_5_17": "5-17",
        "AGE_18_21": "18-21",
        "AGE_22_29": "22-29",
        "AGE_30_39": "30-39",
        "AGE_40_49": "40-49",
        "AGE_50_64": "50-64",
        "AGE_65_UP": "> 64"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    ageGraph: function (data) {

      var maxData = d3.max(arrayUtils.map(data, function (item) {return item.population;}));

      var x = d3.scale.linear()
        .domain([0, maxData])
        .range([0, 240]);

      d3.select(".censusages")
        .selectAll("div")
        .data(data)
          .enter().append("div")
          .style("width", function(d) { return x(d.population) + "px"; })
          .text(function(d) { return d.name + ": " + d.population; });
    }

  });
});
