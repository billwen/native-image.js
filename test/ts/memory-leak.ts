import {drawRedCountdown} from "./samples";
import * as inspector from "node:inspector";

// Start the profiler
const session = new inspector.Session();
session.connect();
session.post('Profiler.enable', () => {
    session.post('Profiler.start', () => {
        // Run your code here
        drawRedCountdown("2024-12-12T00:00:00Z", "red-countdown.gif");

        // Stop the profiler
        session.post('Profiler.stop', (err, { profile }) => {
            // Log the profile data
            console.log(profile);
        });
    });
});
