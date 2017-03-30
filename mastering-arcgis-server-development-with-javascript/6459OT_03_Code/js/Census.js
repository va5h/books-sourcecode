define([
  "dojo/_base/declare",
  "dojo/_base/array",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/dom-style",

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
  "esri/InfoTemplate"
], function (
  declare, arrayUtils, lang,
  dom, domStyle,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  IdentifyParameters, IdentifyTask,
  InfoTemplate
) {
  return declare([_WidgetBase, _TemplatedMixin, _OnDijitClickMixin], {

    baseClass: "y2k-census",
    declaredClass: "y2k.Census",
    templateString: dijitTemplate,
    constructor: function (options, srcRefNode) {

      if (typeof srcRefNode === "string") {
      	srcRefNode = dom.byId(srcRefNode)
      }
      
      this.identifyTask = new IdentifyTask(options.mapService);
      this.map = options.map || null;
      this.domNode = srcRefNode;
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

      return arrayUtils.map(results, function (result) {
        var feature = result.feature,
          title = result.layerName,
          content;

        switch(title) {
          case "Census Block Points":
            content = BlockTemplate;
            break;
          case "Census Block Group":
            content = BlockGroupTemplate;
            break;
          case "Detailed Counties":
            content = CountyTemplate;
            break;
          case "states":
            content = StateTemplate;
            break;
          default:
            content = "${*}";
        }

        feature.infoTemplate = new InfoTemplate(title, content);

        return feature;
      });
    }

  });
});
