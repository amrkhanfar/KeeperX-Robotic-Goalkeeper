# Advanced tracking loop from ball_tracker_node.py
# Full implementation provided upon request

def process_frame(self):
    img_frame = self.rgb_queue.get()
    frame = img_frame.getCvFrame()
    det = self.detector.detect(frame)
    results = self.tracker.process(det, fps, dt, ts, self.frame_id)
    theta = results['theta_deg']
    # Send if conditions met
    send_angle, reason = self.should_send_angle(theta, results, x)
    if send_angle:
        self.serial_queue.put((theta, t_pub, self.frame_id))
    # Drawing and UI...

# Handles OAK-D camera, undistortion, logging