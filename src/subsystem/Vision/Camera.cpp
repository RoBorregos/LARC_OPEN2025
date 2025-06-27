#include "Camera.hpp"

Camera::Camera() {}

void Camera::update() {
    if (!Serial.available()) return;

    String received = Serial.readStringUntil('\n');
    received.trim();

    // Handle lower camera messages
    if (received.startsWith("OBSTACLE:") || received.startsWith("TREE:") || received.startsWith("STORAGE:")) {
        // Auxiliar function to reset values to 0
        auto resetValues = [&]() {
            object_present_ = false;
            object_too_close_ = false;
            current_distance_ = 0.0f;
            current_offset_x_ = 0.0f;
            current_offset_y_ = 0.0f;
            bean_storage_type = false;
        };

        switch (camera_state_) {
        case CameraState::AVOID_OBSTACLES: {
            if (!received.startsWith("OBSTACLE:")) return;

            if (received == "OBSTACLE: False") {
                resetValues();
                return;
            }

            object_present_ = true;

            int idx = received.indexOf("DISTANCE:");
            if (idx != -1)
                current_distance_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("TOO_CLOSE:");
            if (idx != -1)
                object_too_close_ = received.substring(idx + 10, received.indexOf(',', idx)).trim() == "True";

            idx = received.indexOf("OFFSET_X:");
            if (idx != -1)
                current_offset_x_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("OFFSET_Y:");
            if (idx != -1)
                current_offset_y_ = received.substring(idx + 9).toFloat();

            break;
        }
        case CameraState::FETCH_TREES: {
            if (!received.startsWith("TREE:")) return;

            if (received == "TREE: False") {
                resetValues();
                return;
            }

            object_present_ = true;

            int idx = received.indexOf("DISTANCE:");
            if (idx != -1)
                current_distance_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("TOO_CLOSE:");
            if (idx != -1)
                object_too_close_ = received.substring(idx + 10, received.indexOf(',', idx)).trim() == "True";

            idx = received.indexOf("OFFSET_X:");
            if (idx != -1)
                current_offset_x_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("OFFSET_Y:");
            if (idx != -1)
                current_offset_y_ = received.substring(idx + 9).toFloat();

            break;
        }
        case CameraState::FETCH_STOREHOUSE_MADURO:
        case CameraState::FETCH_STOREHOUSE_SOBREMADURO: {
            if (!received.startsWith("STORAGE:")) return;

            if (received == "STORAGE: False") {
                resetValues();
                return;
            }

            object_present_ = true;

            int idx = received.indexOf("TYPE:");
            if (idx != -1) {
                String type = received.substring(idx + 5, received.indexOf(',', idx)).trim();
                bean_storage_type = (type == "SOBREMADURO");
            }

            idx = received.indexOf("DISTANCE:");
            if (idx != -1)
                current_distance_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("TOO_CLOSE:");
            if (idx != -1)
                object_too_close_ = received.substring(idx + 10, received.indexOf(',', idx)).trim() == "True";

            idx = received.indexOf("OFFSET_X:");
            if (idx != -1)
                current_offset_x_ = received.substring(idx + 9, received.indexOf(',', idx)).toFloat();

            idx = received.indexOf("OFFSET_Y:");
            if (idx != -1)
                current_offset_y_ = received.substring(idx + 9).toFloat();

            break;
        }
        default:
            break;
        }
    }
    // Handle upper camera messages
    else if (received.startsWith("Type:")) {
        int typeIndex = received.indexOf("Type:");
        int offsetXIndex = received.indexOf("OffsetX:");
        int offsetYIndex = received.indexOf("OffsetY:");

        // Process type
        BeanConstants::BeanType newType = BeanConstants::NONE;
        if (typeIndex != -1) {
            int commaIndex = received.indexOf(",", typeIndex);
            String typeStr = (commaIndex != -1) ?
                            received.substring(typeIndex + 5, commaIndex) :
                            received.substring(typeIndex + 5);
            typeStr.trim();

            if (typeStr == "MADURO") newType = BeanConstants::MADURO;
            else if (typeStr == "SOBREMADURO") newType = BeanConstants::SOBREMADURO;
        }

        current_bean_type_ = newType;

        // If type=NONE, offsets are 0.0
        if (current_bean_type_ == BeanConstants::NONE) {
            upper_offset_x_ = 0.0;
            upper_offset_y_ = 0.0;
            return;
        }

        // Process OffsetX
        if (offsetXIndex != -1) {
            int commaIndex = received.indexOf(",", offsetXIndex);
            String value = (commaIndex != -1) ?
                          received.substring(offsetXIndex + 8, commaIndex) :
                          received.substring(offsetXIndex + 8);
            value.trim();
            upper_offset_x_ = value.toFloat();
        }

        // Process OffsetY
        if (offsetYIndex != -1) {
            int commaIndex = received.indexOf(",", offsetYIndex);
            String value = (commaIndex != -1) ?
                          received.substring(offsetYIndex + 8, commaIndex) :
                          received.substring(offsetYIndex + 8);
            value.trim();
            upper_offset_y_ = value.toFloat();
        }
    }
}

// Lower camera methods
void Camera::setState(int state) {
    camera_state_ = static_cast<CameraState>(state);
    Serial.print("Camera state: ");
    Serial.println(state);
}

float Camera::getOffset_X() {
    return current_offset_x_;
}

float Camera::getOffset_Y() {
    return current_offset_y_;
}

float Camera::getObjectDistance() {
    return current_distance_;
}

bool Camera::objectPresent() {
    return object_present_;
}

bool Camera::isObjectTooClose() {
    return object_too_close_ || current_distance_ <= 10.0;
}

// Upper camera methods
BeanConstants::BeanType Camera::getBeanType() {
    return current_bean_type_;
}

float Camera::getBeanOffset_X() {
    return upper_offset_x_;
}

float Camera::getBeanOffset_Y() {
    return upper_offset_y_;
} 