% Calibration data
dist = [5 8 10 12 14 17 20 24 26 28 30 32 35 39 42 45 47 49 54 56 58 60];
reading = [565 512 447 390 343 294 252 211 195 183 172 163 154 138 130 122 117 113 105 101 96 96];

% Prep the readings figure
figure(1); clf; hold on;
title('IR Sensor Distance Readings');
xlabel('Distance (in)');
ylabel('Digital reading');

% Plot the points
plot(dist, reading, 'go');

% Prep the calibration curve figure
figure(2); clf; hold on;
title('IR Sensor Distance Calibration Curve');
xlabel('Digital reading');
ylabel('Distance (in)');

% Plot the points with the dependent and independent variable flipped
plot(reading, dist, 'go');

% Fit a parabola
parab_coeffs = polyfit(reading, dist, 4);

% Plot the parabola
parab_x = linspace(min(reading), max(reading))';
parab_A = [parab_x.^4 parab_x.^3 parab_x.^2 parab_x ones(length(parab_x),1)];
parab_y = parab_A * parab_coeffs';
plot(parab_x, parab_y, 'b');


%% Make an error plot
calculated = [6 9.3 10.5 14 16 18.5 24 31];
actual = [7 10 12 14 17 21 26 33];

figure(3); clf; hold on;
plot(calculated, actual, 'rx');
x_y = [min(actual) max(actual)];
plot(x_y, x_y);
title('Calculated Distance vs Actual Distance');
xlabel('Calculated Distance (in)');
ylabel('Actual Distance (in)');
axis square;
legend('Measured points', 'Perfect fit line');