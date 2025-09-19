# Testing in simulation from simulation.py
# Full implementation provided upon request

class TrackerSimulator:
    def run_simulation(self, detections, dts=None, fps=60):
        for i, det in enumerate(detections):
            results = self.tracker.process(det, 1.0/dt, dt, sim_ts, i)
            # Plot vx, theta...
        self.plot_logs()

def plot_logs(self):
    import matplotlib.pyplot as plt
    plt.plot(self.vx_log, label='Kalman vx')
    plt.plot(self.theta_log, label='θ (deg)')
    plt.show()

# Stress tests velocity thresholds, occlusion