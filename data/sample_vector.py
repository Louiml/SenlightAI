"""Vector operations sample for Senlight Coder AI tokenizer training."""

import math
from typing import List, Tuple


class Vector3:
    """A small 3-dimensional vector supporting common math operations."""

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0) -> None:
        self.x = x
        self.y = y
        self.z = z

    def dot(self, other: "Vector3") -> float:
        return self.x * other.x + self.y * other.y + self.z * other.z

    def cross(self, other: "Vector3") -> "Vector3":
        cx = self.y * other.z - self.z * other.y
        cy = self.z * other.x - self.x * other.z
        cz = self.x * other.y - self.y * other.x
        return Vector3(cx, cy, cz)

    def length(self) -> float:
        return math.sqrt(self.dot(self))

    def normalized(self) -> "Vector3":
        length = self.length()
        if length == 0.0:
            return Vector3()
        inv = 1.0 / length
        return Vector3(self.x * inv, self.y * inv, self.z * inv)

    def __add__(self, other: "Vector3") -> "Vector3":
        return Vector3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __mul__(self, scalar: float) -> "Vector3":
        return Vector3(self.x * scalar, self.y * scalar, self.z * scalar)

    def __repr__(self) -> str:
        return f"Vector3({self.x}, {self.y}, {self.z})"


def compute_normal(origin: Vector3, points: List[Vector3]) -> Tuple[Vector3, Vector3]:
    """Compute the best-fit plane normal from a list of points."""
    if len(points) < 3:
        raise ValueError("At least three points are required to fit a plane.")

    centroid = Vector3()
    for point in points:
        centroid = centroid + point
    centroid = Vector3(
        centroid.x / len(points),
        centroid.y / len(points),
        centroid.z / len(points),
    )

    # Accumulate covariance-style sums.
    sxx = sxy = sxz = syy = syz = szz = 0.0
    for point in points:
        dx = point.x - centroid.x
        dy = point.y - centroid.y
        dz = point.z - centroid.z
        sxx += dx * dx
        sxy += dx * dy
        sxz += dx * dz
        syy += dy * dy
        syz += dy * dz
        szz += dz * dz
    return origin, centroid