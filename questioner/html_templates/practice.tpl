<html>
<head>
  <title>Practice</title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
  <script type="text/javascript">
    var state = 0;
    function answermode() {
      $("#answer").show();
      $("#showanswer").hide();
      $("#difficulties").show();
      state = 1;
    }
    function nextquestion(last_difficulty) {
      window.location = "?q={{NEXTQ}}&la=" + last_difficulty;
    }
    $(document).keydown(function(event) {
      if (state == 0) {
        if (event.which == 32) {  // space
          answermode();
        } 
      } else {
        switch(event.which) {
          case 49:   // 1
            nextquestion(0); break;
          case 50:   // 2
            nextquestion(1); break;
          case 51:   // 3
          case 32:   // space
            nextquestion(2); break;
          case 52:   // 4
            nextquestion(3); break;
        }
      }
    });
    $(document).ready(function() {
      $("#showanswer").click(function() {
        answermode();
      });
    });
  </script>
  <style type="text/css">
    body { 
      background-color: #ADFFBC; 
      font-family: Helvetica, sans-serif;
    }
    div#main { 
      background-color: #ffffff; 
      margin: auto; 
      width: 800px;
      padding: 10px;
      border-radius: 10px;
    }
    .center {
      text-align: center;
    }
    .container {
	overflow: auto;
	width: 100%
    }
    .title {
      font-weight: bold;
      font-size: 150%;
      float: left;
    }
    .questionnum {
      font-size: 100%;
      float: right;
    }
    .qword {
      color: red;
    }
    .aword {
      color: blue;
      font-weight: bold;
    }
    button {
      background-color: #ffad33;
      border-radius: 15px;
      border: 0px;
      padding: 10px 40px;
    }
    button:hover {
      background-color: #b26b00;
    }
  </style>
</head>
<body>
  <div id="main">
    <div class="container">
      <p class="title">Case Practice : Genitive</p>
      <p class="questionnum">Question {{CURQ}} of {{TOTALQ}}</p>
    </div>
  <p>{{QUESTION}}</p>
  <hr>
  <p id="answer" style="display:none">{{ANSWER}}</p>
  <div class="center">
    <button id="showanswer">Show Answer</button>
  </div>
  <div id="difficulties" class="center" style="display:none">
    <button id="d0" onclick="nextquestion(0)">Again</button>
    <button id="d1" onclick="nextquestion(1)">Hard</button>
    <button id="d2" onclick="nextquestion(2)">Medium</button>
    <button id="d3" onclick="nextquestion(3)">Easy</button>
  </div>
  </div>
</body>
</html>
