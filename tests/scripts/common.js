/**
 * Helper methods and variables for capser test suite.
 */

var pwd = require('system').env['PWD'];
var ROOT = pwd;

// Define the url for all tests.
var url = casper.cli.get('url');

// Set some static strings
var CAMPAIGN_SIGNUP_MESSAGE = "You're signed up for";

// Set default viewport for all tests.
casper.options.viewportSize = { width: 1280, height: 1024 };

casper.logAction = function(action) {
  casper.echo(action, "PARAMETER");
}

/**
 * Remove test IPs from flood table (preventing tests from failing after repeated failed logins).
 */
casper.clearFloodTable = function() {
  // Remove failed login attempts from localhost (tests on Vagrant boxes).
  casper.drush(["sql-query", "DELETE FROM flood WHERE identifier LIKE '%127.0.0.1';"]);

  // Remove failed login attempts from QA server IP address.
  casper.drush(["sql-query", "DELETE FROM flood WHERE identifier LIKE '%192.168.1.161';"]);
};

casper.randomEmail = function() {
  return Date.now() + "@example.com";
};

casper.randomPassword = function() {
  return Math.random().toString(36).slice(-8);
};

/**
 * Generate a user with a random email and password.
 */
casper.createTestUser = function() {
  var email = casper.randomEmail();
  var password = casper.randomPassword();
  return casper.createUser(email, password);
};

/**
 * Create a user with the given email and password.
 */
casper.createUser = function(email, password) {
  casper.logAction("Creating user with email '" + email + "' and password '" + password + "'...");
  casper.drush(['user-create', 'CASPER_USER', '--mail=' + email, '--password=' + password]);
  return casper.getUserWithEmail(email, password);
};

casper.getUserWithEmail = function(email, password) {
  casper.logAction("Getting user information for '" + email + "'...");
  var info = casper.drush(['user-information', email], true);
  var userKeys = Object.keys(info);

  var response = {
    uid: info[userKeys[0]].uid,
    username: info[userKeys[0]].name,
    email: info[userKeys[0]].mail,
  }

  if(password) {
    response.password = password;
  }

  return response;
};

casper.deleteUserWithEmail = function(email) {
  casper.logAction("Deleting user with email '" + email + "'...");
  var uid = casper.getUserWithEmail(email).uid;
  casper.deleteUser(uid);
};

casper.deleteUser = function(uid) {
  casper.logAction("Deleting user '" + uid + "'...");
  casper.drush(["user-cancel", uid, "-y"]);
};

casper.createCampaign = function(fixture) {
  casper.logAction("Creating campaign from fixture '" + fixture + "'...");
  var drush_campaign = casper.drush(["campaign-create", "../tests/fixtures/" + fixture]);
  var nid = drush_campaign.replace(/[^0-9]/g, "");

  var data = require(ROOT + "/tests/fixtures/" + fixture);

  return {
    nid: nid,
    url: url + "/node/" + nid,
    data: data
  };
};


casper.deleteAllTestNodes = function(fixture) {
  casper.logAction("Clearing all test nodes...");
  casper.drush(["test-node-delete"]);
};

casper.campaignSignup = function(nid, uid) {
  casper.logAction("Signing user '" + uid + "' up for campaign '" + nid + "'...");
  casper.drush(["php-eval", "dosomething_signup_create(" + nid + ", " + uid + ");"]);
};

// Use to log in before performing a test.
casper.login = function(username, password) {
  casper.logAction("Logging in as '" + username + "'...");

  // Go home and login.
  casper.thenOpen(url + "/user", function() {
    this.fill('form[action="/user/login"]', {
      name: username,
      pass: password
    }, true);
  });

  casper.then(function() {
    if(casper.exists(".messages.error")) {
      casper.log(this.getElementInfo(".messages.error").text, "warning");
    }
  })
}

// Use to log out after completing a test.
casper.logout = function() {
  casper.logAction("Logging out of current user...");

  casper.thenOpen(url + "/user/logout", function() {
    if(casper.exists(".messages.error")) {
      casper.log(this.getElementInfo(".messages.error").text, "warning");
    }
  });
}

// We want to clear session after every test.
// @NOTE: You'll have to do this manually if you override the tearDown
//        method on a particular test.
casper.test.tearDown(function() {
  phantom.clearCookies();
})

