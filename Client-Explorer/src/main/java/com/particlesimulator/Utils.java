package com.particlesimulator;

public class Utils {
    private Utils() {
        // Prevent instantiation
    }

    public static int windowWidth = 1280;
    public static int windowHeight = 720;

    // Records
    public record Position(double x, double y) { 
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            Position position = (Position) o;
            return Double.compare(position.x, x) == 0 && Double.compare(position.y, y) == 0;
        }
    }

}
