/**
 * We configure RequireJS options, paths, and shims here.
 *
 *  - paths: Load any Bower components in here.
 *  - shim: Shim any non-AMD scripts.
 *  - modules: Configure outputted build files.
 *
 *  @see https://github.com/jrburke/r.js/blob/master/build/example.build.js
 */

require.config({
  paths: {
    "almond": "../bower_components/almond/almond",
    "jquery": "../bower_components/jquery/jquery",
    "jquery-once": "../bower_components/jquery-once/jquery.once",
    "jquery-unveil": "../bower_components/unveil/jquery.unveil",
    "neue": "../bower_components/neue/js",
    "mailcheck": "../bower_components/mailcheck/src/mailcheck",
    "lodash": "../bower_components/lodash/dist/lodash",
    "text": "../bower_components/requirejs-text/text"
  },
  stubModules: ["text"],
  shim: {
    "jquery-once": { deps: ["jquery"] },
    "jquery-unveil": { deps: ["jquery"] },
    "mailcheck": { exports: "Kicksend.mailcheck" }
  },
  modules: [
    {
      name: "lib",
      create: true,
      include: [
        "almond",
        "jquery",
        "jquery-once",
        "jquery-unveil",
        "mailcheck",
        "lodash"
      ]
    },
    {
      name: "app",
      insertRequire: ["app"],
      exclude: [
        "almond",
        "jquery",
        "jquery-once",
        "jquery-unveil",
        "mailcheck",
        "lodash"
      ]
    }
  ]
});
