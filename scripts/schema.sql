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
    totalSize INTEGER CHECK (totalSize >= 0),
    downloadedSize INTEGER DEFAULT 0 CHECK (downloadedSize >= 0),
    filePath TEXT,
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- ------------------------------------------------------
-- segments table
-- ------------------------------------------------------
CREATE TABLE IF NOT EXISTS segments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    downloadId INTEGER NOT NULL,
    startByte INTEGER NOT NULL CHECK (startByte >= 0),
    endByte INTEGER NOT NULL CHECK (endByte >= startByte),
    downloaded INTEGER DEFAULT 0 CHECK (downloaded >= 0),
    percentage REAL DEFAULT 0 CHECK (percentage >= 0 AND percentage <= 100),
    status TEXT NOT NULL CHECK (
        status IN ('pending', 'downloading', 'completed', 'failed')
    ),
    createdAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (downloadId) REFERENCES downloads (id) ON DELETE CASCADE
);

-- ------------------------------------------------------
-- settings table
-- ------------------------------------------------------
CREATE TABLE IF NOT EXISTS settings (
    key TEXT PRIMARY KEY,
    value TEXT,
    updatedAt DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- ======================================================
-- End of GrabX Schema
-- ======================================================
