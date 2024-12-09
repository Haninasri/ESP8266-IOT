import React, { useEffect, useState } from "react";
import { ref, onValue } from "firebase/database";
import {database} from "../firebase";
import { Card, CardContent, Typography } from "@mui/material";

const SensorData = ({ uid }) => {
  const [temperature, setTemperature] = useState(null);
  const [humidity, setHumidity] = useState(null);

  useEffect(() => {
    const tempRef = ref(database, `users/${uid}/temperature`);
    const humidityRef = ref(database, `users/${uid}/humidity`);

    const unsubscribeTemp = onValue(tempRef, (snapshot) => {
      setTemperature(snapshot.val());
    });

    const unsubscribeHumidity = onValue(humidityRef, (snapshot) => {
      setHumidity(snapshot.val());
    });

    return () => {
      unsubscribeTemp();
      unsubscribeHumidity();
    };
  }, [uid]);

  return (
    <Card>
      <CardContent>
        <Typography variant="h5">Sensor Data</Typography>
        <Typography>Temperature: {temperature ?? "Loading..."}°C</Typography>
        <Typography>Humidity: {humidity ?? "Loading..."}%</Typography>
      </CardContent>
    </Card>
  );
};

export default SensorData;