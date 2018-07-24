(function ($) {
  Drupal.behaviors.roomReservations = {
    attach: function(context, settings) {
      
      $('#edit-outlook-signin').once('rooms-form-processed', function() {
        $(this).on('click tap', function(e) {
          signIn();
          return false;
        });
      });
      $('#edit-signout').once('rooms-form-processed', function() {
        $(this).on('click tap', function(e) {
          signOut();
          return false;
        });
      });
      // Graph API endpoint to show user profile
      let meetingRooms = {
        "rsrc-agron-g217":"Agron G217",
        "rsrc-agron-1581":"Agron 1581",
        "rsrc-agron-2016":"Agron 2016",
        "rsrc-agron-2104":"Agron 2104A",
        "rsrc-agron-commons":"Agronomy Commons",
        "rsrc-agron-3022":"Agron 3022",
        "rsrc-agron-3140":"Agron 3140"
      };
      var calendars = new Object();
      var nowDate = new Date();
      var startDateTime = nowDate.toISOString();
      nowDate.setYear(nowDate.getFullYear() + 1);
      var endDateTime = nowDate.toISOString();
      let calendarURLs = Object.keys(meetingRooms).map((id) => {
        return `https://graph.microsoft.com/v1.0/users/${id}@mail.iastate.edu/calendarView?startDateTime=${startDateTime}&endDateTime=${endDateTime}`;
      });
        
      var graphApiEndpoint = "https://graph.microsoft.com/v1.0/me/events";
      var roomCalendarResults = [];

      // Graph API scope used to obtain the access token to read user profile
      var graphAPIScopes = [
        "https://graph.microsoft.com/calendars.read",
        "https://graph.microsoft.com/calendars.readwrite",
        "https://graph.microsoft.com/calendars.readwrite.shared",
        "https://graph.microsoft.com/user.read",
        "https://graph.microsoft.com/user.readbasic.all"
      ];

      // Initialize application
      var userAgentApplication = new Msal.UserAgentApplication(msalconfig.clientID, null, loginCallback, {
          redirectUri: msalconfig.redirectUri
      });
      //Previous version of msal uses redirect url via a property
      if (userAgentApplication.redirectUri) {
          userAgentApplication.redirectUri = msalconfig.redirectUri;
      }

      if (!userAgentApplication.isCallback(window.location.hash) && window.parent === window && !window.opener) {
        var user = userAgentApplication.getUser();
        if (user) {
          document.getElementById("edit-outlook-signin").classList.add("element-invisible");
//          document.getElementById("edit-signout").classList.remove("element-invisible");
          callGraphApi();
        }
      }
      function signIn() {
        userAgentApplication.loginRedirect(graphAPIScopes);

      }
        /**
         * Call the Microsoft Graph API and display the results on the page
         */
      function callGraphApi() {
        var user = userAgentApplication.getUser();
        // Now Call Graph API
        var $graphCallResponseElement = $("#edit-status-element");
        $graphCallResponseElement.removeClass("element-invisible");
        $graphCallResponseElement.append($('<div/>', {id: 'status-connecting', text: 'Connecting...'}));

        // In order to call the Graph API, an access token needs to be acquired.
        // Try to acquire the token used to Query Graph API silently first
        userAgentApplication.acquireTokenSilent(graphAPIScopes)
          .then(function (token) {
              // Call the Web API, sending the acquired token
              callWebApiWithToken("GET", token, $graphCallResponseElement);

          }, function (error) {
              // If the acquireTokenSilent() method fails, then acquire the token interactively via acquireTokenRedirect().
              // In this case, the browser will redirect user back to the Azure Active Directory v2 Endpoint so the user 
              // can re-type the current username and password and/ or give consent to new permissions your application is requesting.
              // After authentication/ authorization completes, this page will be reloaded again and callGraphApi() will be called.
              // Then, acquireTokenSilent will then acquire the token silently, the Graph API call results will be made and results will be displayed in the page.
              if (error) {
                  userAgentApplication.acquireTokenRedirect(graphAPIScopes);
              }
          });

      };

      /**
       * Show an error message in the page
       * @param {string} endpoint - the endpoint used for the error message
       * @param {string} error - the error string
       * @param {object} errorElement - the HTML element in the page to display the error
       */
      function showError(endpoint, error, errorDesc) {
          var formattedError = JSON.stringify(error, null, 4);
          if (formattedError.length < 3) {
              formattedError = error;
          }
          document.getElementById("edit-status-element").classList.remove("element-invisible");
          document.getElementById("edit-status-element").innerHTML = "An error has occurred:<br/>Endpoint: " + endpoint + "<br/>Error: " + formattedError + "<br/>" + errorDesc;
          console.error(error);
      }

      /**
       * Callback method from sign-in: if no errors, call callGraphApi() to show results.
       * @param {string} errorDesc - If error occur, the error message
       * @param {object} token - The token received from login
       * @param {object} error - The error 
       * @param {string} tokenType - The token type: For loginRedirect, tokenType = "id_token". For acquireTokenRedirect, tokenType:"access_token"
       */
      function loginCallback(errorDesc, token, error, tokenType) {
          if (errorDesc) {
              showError(msal.authority, error, errorDesc);
          }
      }

      function processEvent(event, roomID, token) {
        var $calendarDiv = $('.outlook-calendar.outlook-calendar-'+roomID);
        switch(event.type) {
          case 'singleInstance':
            calendars[roomID].single.push(event);
          break;
          case 'occurrence':
            seriesID = event.seriesMasterId;
            // see if we already have the master event for this series
            if (!(calendars[roomID].masters.hasOwnProperty(seriesID))) {
            // if not, get it and add it to the array
              var options = setupWebApiOptions("GET", token);
              var url = 'https://graph.microsoft.com/v1.0/users/' + roomID + '@mail.iastate.edu/events/' +seriesID;
              calendars[roomID].masters[seriesID] = fetch(url, options).then((response) => response.json());
            }
          break;
        }
      }
      
      function fetchPaged(url, roomID, options, token) {
        let p = new Promise((resolve, reject) => {
          fetch(url, options).then((response) => {
            response.json().then(data => {
              data.value.forEach((event) => {
                processEvent(event, roomID, token);
              });
              // calendars[roomID] = calendars[roomID].concat(data.value);
              if (data["@odata.nextLink"]) {
                return fetchPaged(data["@odata.nextLink"], roomID, options, token).then((response2) => {
                  if (response2.status == 200) {
                    response2.json().then(data2 => {
                      //calendars[roomID] = calendars[roomID].concat(data2.value);
                      data.value.forEach((event) => {
                        processEvent(event, roomID, token);
                      });

                      resolve({events: calendars[roomID], roomID: roomID});
                    });
                  } else {
                    resolve({events: calendars[roomID], roomID: roomID});
                  }
                }, reject);
              } else {
                resolve({events: calendars[roomID], roomID: roomID});
              }
            });
          }, reject);
        });
        return p;
      }

      function setupWebApiOptions(method, token) {
        var headers = new Headers();
        var bearer = "Bearer " + token;
        headers.append("Authorization", bearer);
        headers.append("Content-Type", 'application/json');
        var options = {
            method: method,
            headers: headers,
        };
        if (method == 'POST') 
          options.body = JSON.stringify(data);
        return options;
      }

      function createEventDiv(event, roomName) {
        var $eventDiv = $('<div>', {class: 'outlook-calendar-event'});
        var $date = $('<p>', {class: 'outlook-calendar-event-data outlook-calendar-event-date'});
        var startTime = new Date(event.start.dateTime+"Z");
        var endTime = new Date(event.end.dateTime+"Z");
        var durationMs = (endTime - startTime);
        var durationMin = Math.floor(durationMs  / 60000);
        $date.text(startTime.toLocaleDateString("en-US", {hour: 'numeric', minute: '2-digit', second: '2-digit'}));
        $date.appendTo($eventDiv);
        var $title = $('<p>', {class: 'outlook-calendar-event-data outlook-calendar-event-title'});
        $title.text(event.subject);
        $title.appendTo($eventDiv);
        var $body = $('<p>', {class: 'outlook-calendar-event-data outlook-calendar-event-title'});
        $body.html(event.body.content);
        $body.appendTo($eventDiv);
        var $duration = $('<p>', {class: 'outlook-calendar-event-data outlook-calendar-event-duration'});
        $duration.text(durationMin + ' minutes');
        $duration.appendTo($eventDiv);
        var $email = $('<p>', {class: 'outlook-calendar-event-data outlook-calendar-event-email'});
        if (event.attendees.length > 1)
          $email.text(event.attendees[1].emailAddress.address);
        $email.appendTo($eventDiv);
        var $form = $('<form/>', {class: 'outlook-event-create', id: 'outlook-event-form-'+event.id});
        var $idInput = $('<input type="hidden">').attr({name: 'eventID', value: event.id}).appendTo($form);
        var $titleInput = $('<input type="hidden">').attr({name: 'title', value: event.subject}).appendTo($form);
        var $timeInput = $('<input type="hidden">').attr({name: 'startTime', value: startTime.toISOString()}).appendTo($form);
        var $roomInput = $('<input type="hidden">').attr({name: 'roomName', value: roomName}).appendTo($form);
        var $descInput = $('<input type="hidden">').attr({name: 'description', value: event.body.content}).appendTo($form);
        var $lengthInput = $('<input type="hidden">').attr({name: 'duration', value: durationMin}).appendTo($form);
        if (event.hasOwnProperty('recurrence') && event.recurrence !== null)
          var $recurrenceInput = $('<input type="hidden">').attr({name: 'recurrence', value: JSON.stringify(event.recurrence)}).appendTo($form);
        if (event.attendees.length > 1)
          var $emailInput = $('<input type="hidden">').attr({name: 'email', value: event.attendees[1].emailAddress.address}).appendTo($form);
        var $createBtn = $('<input type="submit">').attr({name: 'createEvent', value: 'Import Event'}).appendTo($form);
        $form.submit(function(e) {
          e.preventDefault();
          var url = Drupal.settings.basePath + 'room_reservations/outlook-import-event';
          $.ajax({
            type: "POST",
            url: url,
            data: $(e.delegateTarget).serialize(),
            success: function(data) {
              $(e.delegateTarget).closest('div.outlook-calendar-event').hide();
            }
          });
//          return false;
        });
        $form.appendTo($eventDiv);
        return $eventDiv;
      }

      /**
       * Call a Web API using an access token.
       * 
       * @param {any} endpoint - Web API endpoint
       * @param {any} token - Access token
       * @param {object} responseElement - HTML element used to display the results
       * @param {object} showTokenElement = HTML element used to display the RAW access token
       */
      function callWebApiWithToken(method, token, $responseElement, data) {
        $calendarContainer = $('<div>', {id: 'outlook-calendar-status'});
        $calendarContainer.appendTo($responseElement);
        var options = setupWebApiOptions(method, token);
        for (var i=0;i<calendarURLs.length;i++) {
          var roomID = calendarURLs[i].match(/users\/(.*?)\//);
          roomID = roomID[1].split('@');
          calendars[roomID[0]] = {single: [], recurring: {}, masters: {}};
          var $calendarDiv = $('<div>', {class: 'outlook-calendar outlook-calendar-'+roomID[0]});
          $calendarContainer.append($calendarDiv);
          var roomName = meetingRooms[roomID[0]];
          var roomObj = findRoomByName(roomName);
          if (roomObj) {
            var $calendarHeader = $('<h3>', {class: 'outlook-calendar-header'});
            $calendarHeader.text(roomObj.title + ' (' + roomObj.nid + ')');
            $calendarHeader.appendTo($calendarDiv);
          }
          roomCalendarResults[i] = fetchPaged(calendarURLs[i], roomID[0], options, token).then((finishedCalendar) => {
            $responseElement.find('#status-connecting').hide();
            var roomName = meetingRooms[finishedCalendar.roomID];
            finishedCalendar.events.single.sort(function(a,b) {
              if (a.start.dateTime == b.start.dateTime)
                return 0;
              return (a.start.dateTime > b.start.dateTime) ? 1 : -1;
            });
            $calendarDiv = $('.outlook-calendar.outlook-calendar-'+finishedCalendar.roomID);
            finishedCalendar.events.single.forEach(function(event) {
              var $eventDiv = createEventDiv(event, roomName);
              $eventDiv.appendTo($calendarDiv);
            });
            if (Object.keys(finishedCalendar.events.masters).length > 0) {
              var promisedProperties = [];
              var promisedKeys = Object.keys(finishedCalendar.events.masters);
              promisedKeys.forEach((key) => promisedProperties.push(finishedCalendar.events.masters[key]));
              Promise.all(promisedProperties).then((data) => {
                data.forEach((calendarEvent) => {
                  var eventRoomID = calendarEvent['@odata.context'].match(/#users\('(.*?)%40mail/);
                  eventRoomID = eventRoomID[1];
                  calendars[eventRoomID].recurring[calendarEvent.id] = calendarEvent;
                  var $eventDiv = createEventDiv(calendarEvent, roomName);
                  $eventDiv.addClass('recurring');
                  $eventDiv.appendTo($calendarDiv);
                });
              });
            }
          });
        }
        Promise.all(roomCalendarResults).then(function(finishedCalendars) {
          console.log(calendars);
        });
      }

      /**
       * Sign-out the user
       */
      function signOut() {
          userAgentApplication.logout();
      }
      
      function findRoomByName(roomName) {
        for (var i in Drupal.settings.room_reservations.rooms) {
          if (Drupal.settings.room_reservations.rooms[i].title == roomName) {
            return Drupal.settings.room_reservations.rooms[i];
          }
        }
        return false;
      }
    }
  }
})(jQuery);
