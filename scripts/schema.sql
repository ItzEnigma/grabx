-- ======================================================
-- GrabX Database Schema
-- ======================================================
-- Tables:
--   - downloads : Main download tracking
--   - segments  : Individual download segments
--   - settings  : Application configuration
-- ======================================================

PRAGMA foreign_keys = ON;

-- ------------------------------------------------------
-- downloads table
-- ------------------------------------------------------
CREATE TABLE IF NOT EXISTS downloads (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    url TEXT NOT NULL,
    status TEXT NOT NULL CHECK (
        status IN ('pending', 'downloading', 'paused', 'completed', 'failed', 'cancelled')
    ),
    progress REAL DEFAULT 0 CHECK (progress >= 0 AND progress <= 100),
    total_size INTEGER CHECK (total_size >= 0),
    downloaded_size INTEGER DEFAULT 0 CHECK (downloaded_size >= 0),
    file_path TEXT,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- ------------------------------------------------------
-- segments table
-- ------------------------------------------------------
CREATE TABLE IF NOT EXISTS segments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    download_id INTEGER NOT NULL,
    start_byte INTEGER NOT NULL CHECK (start_byte >= 0),
    end_byte INTEGER NOT NULL CHECK (end_byte >= start_byte),
    downloaded INTEGER DEFAULT 0 CHECK (downloaded >= 0),
    percentage REAL DEFAULT 0 CHECK (percentage >= 0 AND percentage <= 100),
    status TEXT NOT NULL CHECK (
        status IN ('pending', 'downloading', 'completed', 'failed')
    ),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (download_id) REFERENCES downloads (id) ON DELETE CASCADE
);

-- ------------------------------------------------------
-- settings table
-- ------------------------------------------------------
CREATE TABLE IF NOT EXISTS settings (
    key TEXT PRIMARY KEY,
    value TEXT,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- ======================================================
-- End of GrabX Schema
-- ======================================================
