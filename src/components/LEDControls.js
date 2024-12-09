import React from "react";
import { ref, set } from "firebase/database";
import {database} from "../firebase";
import { Button, Grid } from "@mui/material";

const LEDControls = ({ uid }) => {
  const toggleLED = (relay, state) => {
    set(ref(database, `users/${uid}/${relay}`), state);
  };

  return (
    <Grid container spacing={2}>
      {[1, 2, 3, 4].map((relay) => (
        <Grid item xs={6} md={3} key={relay}>
          <Button
            variant="contained"
            color="primary"
            onClick={() => toggleLED(`relay${relay}`, 1)}
          >
            Turn ON LED {relay}
          </Button>
          <Button
            variant="outlined"
            color="secondary"
            onClick={() => toggleLED(`relay${relay}`, 0)}
            style={{ marginTop: "8px" }}
          >
            Turn OFF LED {relay}
          </Button>
        </Grid>
      ))}
    </Grid>
  );
};

export default LEDControls;