define([
  "dojo/_base/declare",
  "dojo/_base/array",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/dom-style",
  "dojo/dom-attr",
  "dojo/dom-construct",
  "dojo/json",
  "dojo/on",
  "dojo/query",
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
  "d3/d3"
], function (
  declare, arrayUtils, lang,
  dom, domStyle, domAttr, domConstruct,
  JSON, dojoOn, dojoQuery,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  IdentifyParameters, IdentifyTask,
  InfoTemplate, MarkerSymbol, FillSymbol,
  esriRequest, Query, QueryTask, graphicsUtils, esriConfig,
  d3
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

    postCreate: function () {

      this.queryDropdownData({
	url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/3",
	fields: ["STATE_NAME", "STATE_FIPS"],
	where: "1=1",
      }).then(lang.hitch(this, this._populateDropdown, {
	  selector: this.stateSelect,
	  fieldName: "STATE_NAME",
	  fips: "STATE_FIPS"
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
      //domStyle.set(this.domNode, "display", "block");
      dojoQuery(this.domNode).modal("show");
      this._mapClickHandler = this.map.on("click", lang.hitch(this, this._onMapClick));
    },

    hide: function () {
      //domStyle.set(this.domNode, "display", "none");
      dojoQuery(this.domNode).modal("hide");
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

    // changed
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


    _stateSelectChanged: function () {
      var stateName = this.stateSelect.value;
      if (stateName && stateName.length > 0) {

        this.queryShapeAndData({
	        title: "states",
          url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/3",
          where: "STATE_NAME = '" + stateName + "'"
        }).then(
        lang.hitch(this, this._updateInfoWindowFromQuery),
        function (err) {
          console.log(err);
        });

        // search for counties in that state.
        this.queryDropdownData({
          url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/2",
          fields: ["NAME", "CNTY_FIPS"],
          where: "STATE_NAME = '" + stateName + "'",
        }).then(lang.hitch(this, this._populateDropdown, {
      	  selector: this.countySelect,
      	  fieldName: "NAME",
      	  fips: "CNTY_FIPS"
      	}));
      }
    },

    _countySelectChanged: function () {
      var stateValue = this.stateSelect.value,
        stateFIPS = domAttr.get(dojoQuery(":selected", this.stateSelect)[0], "data-fips"),
        countyValue = this.countySelect.value,
	      countyFIPS = domAttr.get(dojoQuery(":selected", this.countySelect)[0], "data-fips");

      // load the county data
      this.queryShapeAndData({
          title: "Detailed Counties",
	  url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/2",
          where: "STATE_NAME = '" + stateValue + "' AND NAME = '" + countyValue + "'"
        }).then(
        lang.hitch(this, this._updateInfoWindowFromQuery),
        function (err) {
          console.log(err);
        });

      this.queryDropdownData({
        url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/1",
        fields: ["BLKGRP", "STCOFIPS"],
        where: "STATE_FIPS = '" + stateFIPS + "' AND CNTY_FIPS = '" + countyFIPS + "'"
      }).then(
        lang.hitch(this, this._populateDropdown, {
	  selector: this.blockGroupSelect,
	  fieldName: "BLKGRP",
	  fips: "STCOFIPS"
	}),
        function (err) {
          console.log(err);
        });
    },

    _blockGroupSelectChanged: function () {
      var stateFIPS = this.stateSelect.find(":selected").attr("data-fips"),
        countyFIPS = this.countySelect.find(":selected").attr("data-fips"),
	      blockGroup = this.blockGroupSelect.value;
      this.queryShapeAndData({
        title: "Census Block Group",
        url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/1",
        where: "STATE_FIPS = '" + stateFIPS + "' AND CNTY_FIPS = '" + countyFIPS + "' AND BLKGRP = '" + blockGroup + "'"
      }).then(
        lang.hitch(this, this._updateInfoWindowFromQuery),
        function (err) {
          console.log(err);
        });
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
    // @param (string) args.where- Where clause to pass for the Query
    // @param (string) args.title - title to eventually assign to the ItemTemplate
    queryShapeAndData: function (args) {
      var queryTask = new QueryTask(args.url),
	query = new Query();
      query.returnGeometry = true;
      query.outFields = ["*"];
      query.outSpatialReference = this.map.spatialReference;
      query.where = args.where;

      return queryTask.execute(query).addCallback(lang.hitch(this, this._onQueryComplete, args.title));
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

    // @param (object) args - parameters for updating the tool.
    // @param (HTML node) args.selector - the select HTML DOM node that will be repopulated.
    // @param (string) args.fieldName - name of the field to display.
    // @param (string) args.fips - name of the census FIPS code for that feature, used to query next level down
    // @param (esri/tasks/FeatureSet) featureSet - results of a query.
    _populateDropdown: function (args, featureSet) {

      domConstruct.empty(args.selector)
      arrayUtils.forEach(featureSet.features, function (feature) {
	domConstruct.create("option", {
	  "value": feature.attributes[args.fieldName],
	  "innerHTML": feature.attributes[args.fieldName],
	  "data-fips": feature.attributes[args.fips]
	}, args.selector);
      });
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
