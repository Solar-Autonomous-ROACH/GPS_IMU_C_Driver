# import numpy as np
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d.art3d import Line3D
# import matplotlib.animation as animation

# # Define a function to create a 3D cube
# def create_cube():
#     # Define the vertices of the cube
#     points = np.array([[-1, -1, -1],
#                        [1, -1, -1],
#                        [1, 1, -1],
#                        [-1, 1, -1],
#                        [-1, -1, 1],
#                        [1, -1, 1],
#                        [1, 1, 1],
#                        [-1, 1, 1]])
    
#     # Define the edges connecting the vertices
#     edges = [(0, 1), (1, 2), (2, 3), (3, 0), (4, 5), (5, 6),
#              (6, 7), (7, 4), (0, 4), (1, 5), (2, 6), (3, 7)]
    
#     return points, edges

# # Function to apply rotations to the cube
# def rotate_cube(points, roll, pitch, yaw):
#     Rx = np.array([[1, 0, 0],
#                    [0, np.cos(roll), -np.sin(roll)],
#                    [0, np.sin(roll), np.cos(roll)]])
    
#     Ry = np.array([[np.cos(pitch), 0, np.sin(pitch)],
#                    [0, 1, 0],
#                    [-np.sin(pitch), 0, np.cos(pitch)]])
    
#     Rz = np.array([[np.cos(yaw), -np.sin(yaw), 0],
#                    [np.sin(yaw), np.cos(yaw), 0],
#                    [0, 0, 1]])
    
#     # Apply rotations
#     R = np.dot(Rz, np.dot(Ry, Rx))
#     rotated_points = np.dot(points, R.T)
    
#     return rotated_points

# # Set up the figure and 3D axis
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

# # Create the cube
# points, edges = create_cube()
# edge_lines = [Line3D([], [], [], color="b", marker="o") for _ in edges]
# for edge_line in edge_lines:
#     ax.add_line(edge_line)

# # Animation update function
# def update(frame, points, edges, edge_lines):
#     # Read IMU data from file (replace with your own reading mechanism)
#     try:
#         with open("rpy_data.txt", "r") as file:
#             data = file.read().strip().split(',')
#             pitch, roll, yaw = map(float, data)
#     except IOError:
#         print("File not accessible")
#         return
#     except ValueError:
#         print("Error in data format")
#         return

#     # Rotate cube
#     rotated_points = rotate_cube(points, roll, pitch, yaw)

#     # Update the edges of the cube
#     for edge_line, (start, end) in zip(edge_lines, edges):
#         edge_line.set_data([rotated_points[start, 0], rotated_points[end, 0]],
#                            [rotated_points[start, 1], rotated_points[end, 1]])
#         edge_line.set_3d_properties([rotated_points[start, 2], rotated_points[end, 2]])
    
#     # Set the limits of the axes
#     ax.set_xlim([-2, 2])
#     ax.set_ylim([-2, 2])
#     ax.set_zlim([-2, 2])
#     ax.set_xlabel('X')
#     ax.set_ylabel('Y')
#     ax.set_zlabel('Z')

#     return edge_lines

# # Create the animation
# ani = animation.FuncAnimation(fig, update, frames=6, fargs=(points, edges, edge_lines), interval=100, blit=False)

# plt.show()

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Maximum number of points in the path
MAX_POINTS = 7

fig, ax = plt.subplots()
path_line, = ax.plot([], [], 'r-', linewidth=2)  # Path line

# Set initial axis limits (adjust these values based on your expected data range)
initial_lon = 120.0004
initial_lat = [Your initial latitude value]
lon_range = 0.1  # Adjust the range based on expected variation
lat_range = 0.1  # Adjust the range based on expected variation
ax.set_xlim(initial_lon - lon_range/2, initial_lon + lon_range/2)
ax.set_ylim(initial_lat - lat_range/2, initial_lat + lat_range/2)

def update(frame, path_line):
    try:
        with open("rpy_data.txt", "r") as file:
            data = file.readline().strip().split(',')
            _, _, _, lat, lon, _ = map(float, data)
    except (IOError, ValueError):
        print("Error reading data")
        return [path_line]

    # Update the path trace
    xdata = list(path_line.get_xdata())
    ydata = list(path_line.get_ydata())

    # Append new data
    xdata.append(lon)
    ydata.append(lat)

    # Keep only the last MAX_POINTS points
    if len(xdata) > MAX_POINTS:
        xdata = xdata[-MAX_POINTS:]
        ydata = ydata[-MAX_POINTS:]

    path_line.set_data(xdata, ydata)

    # Update the axes limits if needed
    current_x_min, current_x_max = ax.get_xlim()
    current_y_min, current_y_max = ax.get_ylim()
    if lon < current_x_min or lon > current_x_max or lat < current_y_min or lat > current_y_max:
        ax.set_xlim(min(xdata) - lon_range/2, max(xdata) + lon_range/2)
        ax.set_ylim(min(ydata) - lat_range/2, max(ydata) + lat_range/2)

    ax.set_xlabel('Longitude')
    ax.set_ylabel('Latitude')

    return [path_line]

ani = animation.FuncAnimation(fig, update, fargs=(path_line,), interval=100, blit=True)
plt.show()
