// Firebase function for requests
const {onRequest} = require("firebase-functions/v2/https");

// The Firebase Admin SDK to access realtime database.
const {getDatabase} = require("firebase-admin/database");
const admin = require("firebase-admin");
const {ref, set} = require("firebase/database");

const cors = require("cors")({origin: true});

// init app, admin is necessary for Functions
admin.initializeApp();

// Deal with incrementing
// Take the text parameter passed to this HTTP endpoint
exports.increment = onRequest(async (req, res) => {
  cors(req, res, () => {
    const db = getDatabase();
    // request may be empty
    if (!req.query.inc) return res.send("No inc passes");

    const incStat = req.query.inc;
    console.log("Increment value: ", incStat);

    db.ref("/testInc/inc").once("value").then((snap) => {
      if (snap.exists()) {
        const newInc = parseInt(snap.val()) + parseInt(incStat) + "";
        set(ref(db, "testInc/"), {
          inc: newInc,
        });
      } else {
        console.log("inc not found. init new inc");
        set(ref(db, "testInc/"), {
          inc: incStat,
        });
      }
    }).then(() => {
      res.status(200).send("Inc success");
    }).catch((err) => {
      console.log(err);
      res.send("error occurred");
    });
  });
});


// Take the text parameter passed to this HTTP endpoint
exports.setLED = onRequest(async (req, res) => {
  cors(req, res, () => {
    const db = getDatabase();

    // might be empty
    if (!req.query.led) return res.send("No led params");

    // Grab the led parameter.
    const ledStat = req.query.led;

    let ledValue = 0;
    if (ledStat == "1") {
      ledValue = 1;
    } else if (ledStat == "0") {
      ledValue = 0;
    }
    set(ref(db, "testLED/"), {
      led: ledValue,
    }).then(() => {
      res.status(200).send("led success");
    }).catch((err) => {
      console.log(err);
      res.send("error occurred");
    });
  });
});

exports.getLED = onRequest((req, res) => {
  cors(req, res, () => {
    const db = getDatabase();

    db.ref("/testLED/led").once("value").then((snap) => {
      if (snap.exists()) {
        return res.send(snap.val() + "");
      } else {
        return res.send(-1 + "");
      }
    }).catch((err) => {
      console.log(err);
      res.send("error occurred");
    });
  });
});

exports.getInc = onRequest((req, res) => {
  cors(req, res, () => {
    const db = getDatabase();

    db.ref("/testInc/inc").once("value").then((snap) => {
      if (snap.exists()) {
        return res.send(snap.val() + "");
      } else {
        return res.send(-1 + "");
      }
    }).catch((err) => {
      console.log(err);
      res.send("error occurred");
    });
  });
});
