#!/bin/bash

echo "🤖 LARC Vision System - Model IRL Runner"
echo "========================================"

# Function to run the vision system directly (without Docker)
run_direct() {
    echo "Running vision module directly..."
    python main.py
}

# Function to build and run with Docker
run_docker() {
    echo "Building Docker image..."
    docker build -t larc-vision:latest .
    
    if [ $? -eq 0 ]; then
        echo "Running Docker container..."
        # Run with camera access and network host for communication
        docker run --rm -it \
            --device=/dev/video0:/dev/video0 \
            --device=/dev/video1:/dev/video1 \
            --device=/dev/video2:/dev/video2 \
            --network=host \
            --name larc-vision-container \
            larc-vision:latest
    else
        echo "❌ Docker build failed!"
        exit 1
    fi
}

# Function to run existing Docker container
run_docker_existing() {
    echo "Running existing Docker container..."
    docker run --rm -it \
        --device=/dev/video0:/dev/video0 \
        --device=/dev/video1:/dev/video1 \
        --device=/dev/video2:/dev/video2 \
        --network=host \
        --name larc-vision-container \
        larc-vision:latest
}

# Function to stop Docker container
stop_docker() {
    echo "Stopping Docker container..."
    docker stop larc-vision-container 2>/dev/null || echo "No container to stop"
}

# Function to clean Docker resources
clean_docker() {
    echo "Cleaning Docker resources..."
    docker stop larc-vision-container 2>/dev/null
    docker rm larc-vision-container 2>/dev/null
    docker rmi larc-vision:latest 2>/dev/null
    echo "✅ Docker resources cleaned"
}

# Main execution logic
case "${1:-direct}" in
    "direct"|"d")
        run_direct
        ;;
    "docker"|"build")
        run_docker
        ;;
    "run"|"r")
        run_docker_existing
        ;;
    "stop"|"s")
        stop_docker
        ;;
    "clean"|"c")
        clean_docker
        ;;
    "help"|"h"|*)
        echo "Usage: $0 [command]"
        echo ""
        echo "Commands:"
        echo "  direct, d     Run vision system directly (default)"
        echo "  docker, build Build Docker image and run container"
        echo "  run, r        Run existing Docker container"
        echo "  stop, s       Stop running Docker container"
        echo "  clean, c      Clean Docker resources"
        echo "  help, h       Show this help message"
        echo ""
        echo "Examples:"
        echo "  ./run.sh              # Run directly"
        echo "  ./run.sh docker       # Build and run with Docker"
        echo "  ./run.sh run          # Run existing Docker image"
        echo "  ./run.sh stop         # Stop container"
        echo "  ./run.sh clean        # Clean Docker resources"
        ;;
esac