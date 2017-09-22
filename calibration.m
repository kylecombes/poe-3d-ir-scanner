% Calibration data (put points to ignore at beginning)
y = [3 4 5 10 20 30 15 25 40 12 6 7 11 17 35 50 37 48 44];
x = [250 560 550 460 250 170 330 210 140 404 563 530 420 276 155 150 148 145 140];

% Split into points to ignore (out of sensor range) and points to use
x_dirty = x(1);
y_dirty = y(1);
x_clean = x(2:end);
y_clean = y(2:end);

% Prep the figure
figure(1); clf; hold on;
title('IR Sensor Distance Calibration');
ylabel('Distance (in)');
xlabel('Digital reading');

% Plot the points
plot(x_clean, y_clean, 'go');
plot(x_dirty, y_dirty, 'rx');

% Fit a parabola
parab_coeffs = polyfit(x_clean, y_clean, 2);

% Plot the parabola
parab_x = linspace(min(x_clean), max(x_clean))';
parab_A = [parab_x.^2 parab_x ones(length(parab_x),1)];
parab_y = parab_A * parab_coeffs';
plot(parab_x, parab_y, 'b');

% Add a legend
legend('Fit points', 'Discarded points', 'Fit curve');