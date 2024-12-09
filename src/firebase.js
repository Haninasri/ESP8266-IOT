import { initializeApp } from "firebase/app";
import { getAuth, signInWithEmailAndPassword } from "firebase/auth";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
	apiKey: "AIzaSyDe4tK1DS3q8MTnbUE3bZXRz3kUwpB652c",
	authDomain: "esp8266-a-8b851.firebaseapp.com",
	databaseURL: "https://esp8266-a-8b851-default-rtdb.firebaseio.com",
	projectId: "esp8266-a-8b851",
	storageBucket: "esp8266-a-8b851.firebasestorage.app",
	messagingSenderId: "653261709067",
	appId: "1:653261709067:web:31fb83f298298f1c024c0e",
	measurementId: "G-YQJ72GCPJM"
  };

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const database = getDatabase(app);

export { auth, database, signInWithEmailAndPassword };