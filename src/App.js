import React, { useState, useEffect } from "react";
import { auth, signInWithEmailAndPassword } from "./firebase";
import LEDControls from "./components/LEDControls";
import SensorData from "./components/SensorData";
import { Container, Typography, Box, Button, TextField } from "@mui/material";

const App = () => {
  const [user, setUser] = useState(null);
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");

  const handleLogin = () => {
    signInWithEmailAndPassword(auth, email, password)
      .then((userCredential) => {
        setUser(userCredential.user);
      })
      .catch((error) => {
        console.error("Login error:", error.message);
      });
  };

  if (!user) {
    return (
      <Container>
        <Typography variant="h4" gutterBottom>
          Login
        </Typography>
        <Box my={2}>
          <TextField
            label="Email"
            variant="outlined"
            fullWidth
            value={email}
            onChange={(e) => setEmail(e.target.value)}
          />
        </Box>
        <Box my={2}>
          <TextField
            label="Password"
            type="password"
            variant="outlined"
            fullWidth
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
        </Box>
        <Button variant="contained" color="primary" onClick={handleLogin}>
          Login
        </Button>
      </Container>
    );
  }

  return (
    <Container>
      <Typography variant="h3" align="center" gutterBottom>
        ESP8266 Firebase Dashboard
      </Typography>
      <Box my={4}>
        <SensorData uid={user.uid} />
      </Box>
      <Box my={4}>
        <LEDControls uid={user.uid} />
      </Box>
    </Container>
  );
};

export default App;