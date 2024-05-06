-- Creating a table
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Inserting data
INSERT INTO users (username, email) VALUES ('john_doe', 'john.doe@example.com');

-- Selecting data
SELECT * FROM users;

-- Updating data
UPDATE users SET username = 'jane_doe' WHERE id = 1;

-- Deleting data
DELETE FROM users WHERE id = 1;
