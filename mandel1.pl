#!/usr/bin/perl

use 5.030;
use strict;
use warnings;

my ($WIDTH, $HEIGHT) = (512, 512);
my ($X_MIN, $X_MAX) = (-1.5 - 0.7, 1.5 - 0.7);
my ($Y_MIN, $Y_MAX) = (-1.5, 1.5);

my $x_step = ($X_MAX - $X_MIN) / $WIDTH;
my $y_step = ($Y_MAX - $Y_MIN) / $HEIGHT;

print "P6\n";
print "# created by $0 at " . localtime . "\n";
print "$WIDTH $HEIGHT\n";
print "255\n";
binmode STDOUT;

for my $y_pixel (0..$HEIGHT-1) {
	for my $x_pixel (0..$WIDTH-1) {
		my $c_x = $X_MIN + $x_step / 2 + $x_pixel * $x_step;
		my $c_y = $Y_MIN + ($y_pixel + 1/2) * $y_step;

		my ($z_x, $z_y) = ($c_x, $c_y);
		for (1..256) {
			# z <- z^2 + c
			($z_x, $z_y) = ($z_x**2 - $z_y**2 + $c_x, 2*$z_x*$z_y + $c_y);
			($z_x, $z_y) = ($z_x**2 - $z_y**2 + $c_x, 2*$z_x*$z_y + $c_y);
			($z_x, $z_y) = ($z_x**2 - $z_y**2 + $c_x, 2*$z_x*$z_y + $c_y);
			($z_x, $z_y) = ($z_x**2 - $z_y**2 + $c_x, 2*$z_x*$z_y + $c_y);
			last if $z_x > 2 or $z_y > 2;
		}

		if (sqrt($z_x**2 + $z_y**2) < 2) {
			print pack("CCC", 0, 255, 255);
		} else {
			print pack("CCC", 180, 0, 0);
		}
		#print pack("CCC", $x_pixel / ($WIDTH-1) * 255, 0, 0);
		#print pack("CCC", 0, $y_pixel / ($WIDTH-1) * 255, 0, 0);
		#print pack("CCC", 255*($c_x / 4 + 0.5), 255*($c_y / 4 + 0.5), 0);
	}
}
