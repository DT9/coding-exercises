# Twenty CRM Security Hardening Guide

**Date:** July 28, 2025  
**Version:** 1.0  
**Scope:** Twenty CRM Application Security Hardening  
**Target Environment:** Production Deployment  

---

## Executive Summary

This security hardening guide provides comprehensive recommendations to secure the Twenty CRM application based on identified vulnerabilities and industry best practices. The implementation of these controls will significantly reduce the attack surface and improve the overall security posture.

### Hardening Objectives
- Eliminate critical security vulnerabilities
- Implement defense-in-depth security controls
- Establish secure configuration standards
- Enable comprehensive security monitoring
- Ensure compliance with security frameworks

### Risk Reduction Goals
- **Critical Risk:** Eliminate all critical vulnerabilities
- **High Risk:** Reduce by 90% through proper controls
- **Medium Risk:** Mitigate through layered security
- **Overall Security Posture:** Achieve enterprise-grade security

---

## Infrastructure Hardening

### 1. Network Security Controls

#### 1.1 Web Application Firewall (WAF)
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```yaml
# AWS WAF Configuration Example
WAF_Rules:
  - Name: "SQLInjectionRule"
    Type: "SQL_INJECTION_MATCH"
    Action: "BLOCK"
  
  - Name: "XSSRule"
    Type: "XSS_MATCH"
    Action: "BLOCK"
  
  - Name: "RateLimitRule"
    Type: "RATE_BASED"
    Limit: 2000
    Period: 5
    Action: "BLOCK"
  
  - Name: "IPWhitelistRule"
    Type: "IP_MATCH"
    Action: "ALLOW"
    IPs: ["192.168.1.0/24", "10.0.0.0/8"]
```

#### 1.2 Network Segmentation
**Priority:** High  
**Implementation Timeline:** Week 1  

```hcl
# Terraform VPC Configuration
resource "aws_vpc" "twenty_vpc" {
  cidr_block           = "10.0.0.0/16"
  enable_dns_hostnames = true
  enable_dns_support   = true
  
  tags = {
    Name = "twenty-crm-vpc"
  }
}

# Private subnets for application
resource "aws_subnet" "private_app" {
  count             = 2
  vpc_id            = aws_vpc.twenty_vpc.id
  cidr_block        = "10.0.${count.index + 1}.0/24"
  availability_zone = data.aws_availability_zones.available.names[count.index]
  
  tags = {
    Name = "twenty-private-app-${count.index + 1}"
  }
}

# Private subnets for database
resource "aws_subnet" "private_db" {
  count             = 2
  vpc_id            = aws_vpc.twenty_vpc.id
  cidr_block        = "10.0.${count.index + 10}.0/24"
  availability_zone = data.aws_availability_zones.available.names[count.index]
  
  tags = {
    Name = "twenty-private-db-${count.index + 1}"
  }
}
```

#### 1.3 Security Groups
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```hcl
# Application Load Balancer Security Group
resource "aws_security_group" "alb_sg" {
  name_prefix = "twenty-alb-"
  vpc_id      = aws_vpc.twenty_vpc.id

  ingress {
    from_port   = 443
    to_port     = 443
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  ingress {
    from_port   = 80
    to_port     = 80
    protocol    = "tcp"
    cidr_blocks = ["0.0.0.0/0"]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

# Application Security Group
resource "aws_security_group" "app_sg" {
  name_prefix = "twenty-app-"
  vpc_id      = aws_vpc.twenty_vpc.id

  ingress {
    from_port       = 3000
    to_port         = 3000
    protocol        = "tcp"
    security_groups = [aws_security_group.alb_sg.id]
  }

  egress {
    from_port   = 0
    to_port     = 0
    protocol    = "-1"
    cidr_blocks = ["0.0.0.0/0"]
  }
}

# Database Security Group
resource "aws_security_group" "db_sg" {
  name_prefix = "twenty-db-"
  vpc_id      = aws_vpc.twenty_vpc.id

  ingress {
    from_port       = 5432
    to_port         = 5432
    protocol        = "tcp"
    security_groups = [aws_security_group.app_sg.id]
  }
}
```

---

## Application Security Hardening

### 2. Authentication and Authorization

#### 2.1 JWT Security Configuration
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
// Enhanced JWT Configuration
const jwtConfig = {
  secret: process.env.JWT_SECRET, // 256-bit random key
  options: {
    algorithm: 'HS256',
    expiresIn: '15m', // Short-lived tokens
    issuer: 'twenty-crm',
    audience: 'twenty-crm-users',
    notBefore: '0',
    clockTolerance: 30
  },
  refreshToken: {
    expiresIn: '7d',
    httpOnly: true,
    secure: true,
    sameSite: 'strict'
  }
};

// JWT Middleware with proper validation
const authenticateJWT = (req, res, next) => {
  const authHeader = req.headers.authorization;
  
  if (!authHeader || !authHeader.startsWith('Bearer ')) {
    return res.status(401).json({ error: 'Unauthorized - Missing token' });
  }
  
  const token = authHeader.substring(7);
  
  try {
    const decoded = jwt.verify(token, jwtConfig.secret, jwtConfig.options);
    req.user = decoded;
    next();
  } catch (error) {
    if (error.name === 'TokenExpiredError') {
      return res.status(401).json({ error: 'Token expired' });
    }
    return res.status(403).json({ error: 'Invalid token' });
  }
};
```

#### 2.2 Role-Based Access Control (RBAC)
**Priority:** High  
**Implementation Timeline:** Week 1  

```javascript
// RBAC Implementation
const roles = {
  ADMIN: 'admin',
  USER: 'user',
  VIEWER: 'viewer'
};

const permissions = {
  [roles.ADMIN]: [
    'users:create', 'users:read', 'users:update', 'users:delete',
    'companies:create', 'companies:read', 'companies:update', 'companies:delete',
    'admin:access'
  ],
  [roles.USER]: [
    'companies:create', 'companies:read', 'companies:update',
    'contacts:create', 'contacts:read', 'contacts:update'
  ],
  [roles.VIEWER]: [
    'companies:read', 'contacts:read'
  ]
};

const authorize = (requiredPermission) => {
  return (req, res, next) => {
    const userRole = req.user.role;
    const userPermissions = permissions[userRole] || [];
    
    if (!userPermissions.includes(requiredPermission)) {
      return res.status(403).json({ error: 'Insufficient permissions' });
    }
    
    next();
  };
};

// Usage example
app.get('/api/admin', authenticateJWT, authorize('admin:access'), adminController);
```

### 3. GraphQL Security

#### 3.1 GraphQL Hardening
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
const { ApolloServer } = require('apollo-server-express');
const depthLimit = require('graphql-depth-limit');
const costAnalysis = require('graphql-cost-analysis');

const server = new ApolloServer({
  typeDefs,
  resolvers,
  // Disable introspection in production
  introspection: process.env.NODE_ENV !== 'production',
  playground: process.env.NODE_ENV !== 'production',
  
  // Security validations
  validationRules: [
    depthLimit(10), // Limit query depth
    costAnalysis({
      maximumCost: 1000,
      defaultCost: 1,
      createError: (max, actual) => {
        return new Error(`Query cost ${actual} exceeds maximum cost ${max}`);
      }
    })
  ],
  
  // Rate limiting
  plugins: [
    {
      requestDidStart() {
        return {
          willSendResponse(requestContext) {
            // Log queries for monitoring
            console.log('GraphQL Query:', requestContext.request.query);
          }
        };
      }
    }
  ],
  
  // Context with authentication
  context: ({ req }) => {
    const token = req.headers.authorization?.replace('Bearer ', '');
    let user = null;
    
    if (token) {
      try {
        user = jwt.verify(token, process.env.JWT_SECRET);
      } catch (error) {
        throw new AuthenticationError('Invalid token');
      }
    }
    
    return { user };
  }
});
```

#### 3.2 Query Complexity Analysis
**Priority:** High  
**Implementation Timeline:** Week 1  

```javascript
const { createComplexityLimitRule } = require('graphql-validation-complexity');

const complexityLimitRule = createComplexityLimitRule(1000, {
  scalarCost: 1,
  objectCost: 2,
  listFactor: 10,
  introspectionCost: 1000
});

// Field-level complexity scoring
const typeDefs = gql`
  type Query {
    # Simple query - low complexity
    me: User @complexity(value: 1)
    
    # Expensive query - high complexity
    allUsers: [User] @complexity(value: 100)
    
    # Nested query with multipliers
    companies: [Company] @complexity(value: 10, multipliers: ["limit"])
  }
`;
```

### 4. Input Validation and Sanitization

#### 4.1 Comprehensive Input Validation
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
const Joi = require('joi');
const xss = require('xss');
const { body, param, query, validationResult } = require('express-validator');

// Input sanitization middleware
const sanitizeInput = (req, res, next) => {
  const sanitize = (obj) => {
    if (typeof obj === 'string') {
      return xss(obj);
    }
    if (typeof obj === 'object' && obj !== null) {
      for (const key in obj) {
        obj[key] = sanitize(obj[key]);
      }
    }
    return obj;
  };
  
  req.body = sanitize(req.body);
  req.query = sanitize(req.query);
  req.params = sanitize(req.params);
  next();
};

// Validation schemas
const userSchema = Joi.object({
  email: Joi.string().email().required().max(255),
  password: Joi.string().min(8).max(128).pattern(new RegExp('^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#\$%\^&\*])')).required(),
  firstName: Joi.string().min(1).max(50).required(),
  lastName: Joi.string().min(1).max(50).required(),
  role: Joi.string().valid('admin', 'user', 'viewer').default('user')
});

const companySchema = Joi.object({
  name: Joi.string().min(1).max(255).required(),
  website: Joi.string().uri().optional(),
  industry: Joi.string().max(100).optional(),
  size: Joi.number().integer().min(1).max(1000000).optional()
});

// Validation middleware
const validateInput = (schema) => {
  return (req, res, next) => {
    const { error, value } = schema.validate(req.body);
    if (error) {
      return res.status(400).json({
        error: 'Validation failed',
        details: error.details.map(d => d.message)
      });
    }
    req.body = value;
    next();
  };
};
```

#### 4.2 SQL Injection Prevention
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
// Parameterized queries with TypeORM
import { getRepository } from 'typeorm';

// Safe query implementation
const getUserById = async (id) => {
  const userRepository = getRepository(User);
  
  // Using parameterized query
  return await userRepository.findOne({
    where: { id: parseInt(id) },
    select: ['id', 'email', 'firstName', 'lastName', 'role']
  });
};

// Safe search implementation
const searchCompanies = async (searchTerm) => {
  const companyRepository = getRepository(Company);
  
  return await companyRepository
    .createQueryBuilder('company')
    .where('company.name ILIKE :search', { search: `%${searchTerm}%` })
    .orWhere('company.industry ILIKE :search', { search: `%${searchTerm}%` })
    .take(50) // Limit results
    .getMany();
};
```

### 5. Security Headers Implementation

#### 5.1 Comprehensive Security Headers
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
const helmet = require('helmet');

app.use(helmet({
  // Content Security Policy
  contentSecurityPolicy: {
    directives: {
      defaultSrc: ["'self'"],
      scriptSrc: [
        "'self'",
        "'unsafe-inline'", // Only if absolutely necessary
        "https://cdn.jsdelivr.net",
        "https://unpkg.com"
      ],
      styleSrc: [
        "'self'",
        "'unsafe-inline'",
        "https://fonts.googleapis.com"
      ],
      fontSrc: [
        "'self'",
        "https://fonts.gstatic.com"
      ],
      imgSrc: [
        "'self'",
        "data:",
        "https:",
        "blob:"
      ],
      connectSrc: [
        "'self'",
        "https://api.twenty.com"
      ],
      frameSrc: ["'none'"],
      objectSrc: ["'none'"],
      upgradeInsecureRequests: []
    }
  },
  
  // HTTP Strict Transport Security
  hsts: {
    maxAge: 31536000, // 1 year
    includeSubDomains: true,
    preload: true
  },
  
  // X-Frame-Options
  frameguard: {
    action: 'deny'
  },
  
  // X-Content-Type-Options
  noSniff: true,
  
  // Referrer Policy
  referrerPolicy: {
    policy: 'strict-origin-when-cross-origin'
  },
  
  // Permissions Policy
  permissionsPolicy: {
    features: {
      geolocation: ["'none'"],
      microphone: ["'none'"],
      camera: ["'none'"],
      payment: ["'none'"],
      usb: ["'none'"]
    }
  }
}));

// Remove server identification
app.disable('x-powered-by');

// Custom security headers
app.use((req, res, next) => {
  res.setHeader('X-XSS-Protection', '1; mode=block');
  res.setHeader('X-Download-Options', 'noopen');
  res.setHeader('X-Permitted-Cross-Domain-Policies', 'none');
  next();
});
```

### 6. CORS Configuration

#### 6.1 Secure CORS Implementation
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```javascript
const cors = require('cors');

const corsOptions = {
  origin: function (origin, callback) {
    // Allow requests with no origin (mobile apps, etc.)
    if (!origin) return callback(null, true);
    
    const allowedOrigins = [
      'https://twenty.yourdomain.com',
      'https://app.yourdomain.com',
      'https://admin.yourdomain.com'
    ];
    
    if (process.env.NODE_ENV === 'development') {
      allowedOrigins.push('http://localhost:3000', 'http://localhost:3001');
    }
    
    if (allowedOrigins.indexOf(origin) !== -1) {
      callback(null, true);
    } else {
      callback(new Error('Not allowed by CORS'));
    }
  },
  
  credentials: true, // Allow cookies
  optionsSuccessStatus: 200, // For legacy browser support
  
  methods: ['GET', 'POST', 'PUT', 'DELETE', 'PATCH'],
  allowedHeaders: [
    'Origin',
    'X-Requested-With',
    'Content-Type',
    'Accept',
    'Authorization',
    'X-Access-Token'
  ],
  
  exposedHeaders: ['X-Total-Count'], // Only expose necessary headers
  
  maxAge: 86400 // Cache preflight for 24 hours
};

app.use(cors(corsOptions));
```

---

## Database Security Hardening

### 7. PostgreSQL Security Configuration

#### 7.1 Database Connection Security
**Priority:** Critical  
**Implementation Timeline:** Immediate  

```yaml
# Docker Compose with Security Enhancements
version: '3.8'
services:
  db:
    image: postgres:16
    environment:
      POSTGRES_DB: ${PG_DATABASE_NAME}
      POSTGRES_USER: ${PG_DATABASE_USER}
      POSTGRES_PASSWORD: ${PG_DATABASE_PASSWORD}
      POSTGRES_INITDB_ARGS: "--auth-host=scram-sha-256"
    volumes:
      - db-data:/var/lib/postgresql/data
      - ./postgresql.conf:/etc/postgresql/postgresql.conf
      - ./pg_hba.conf:/etc/postgresql/pg_hba.conf
    command: postgres -c config_file=/etc/postgresql/postgresql.conf
    ports: [] # Remove external port exposure
    networks:
      - internal
    restart: unless-stopped
    
    # Security constraints
    cap_drop:
      - ALL
    cap_add:
      - CHOWN
      - DAC_OVERRIDE
      - FOWNER
      - SETGID
      - SETUID
    
    # Resource limits
    deploy:
      resources:
        limits:
          memory: 2G
          cpus: '1.0'
```

#### 7.2 PostgreSQL Configuration
**Priority:** High  
**Implementation Timeline:** Week 1  

```conf
# postgresql.conf security settings
ssl = on
ssl_cert_file = '/etc/ssl/certs/server.crt'
ssl_key_file = '/etc/ssl/private/server.key'
ssl_ca_file = '/etc/ssl/certs/ca.crt'

# Authentication
password_encryption = scram-sha-256
db_user_namespace = off

# Logging
log_connections = on
log_disconnections = on
log_failed_authentication_attempts = on
log_statement = 'mod'
log_min_duration_statement = 1000

# Resource limits
max_connections = 100
shared_buffers = 256MB
effective_cache_size = 1GB

# Security
shared_preload_libraries = 'pg_stat_statements'
track_activities = on
track_counts = on
```

```conf
# pg_hba.conf access control
# TYPE  DATABASE        USER            ADDRESS                 METHOD
local   all             postgres                                peer
local   all             twenty_user                             scram-sha-256
host    twenty_db       twenty_user     10.0.0.0/16             scram-sha-256
host    all             all             127.0.0.1/32            reject
host    all             all             ::1/128                 reject
```

#### 7.3 Database User Privileges
**Priority:** High  
**Implementation Timeline:** Week 1  

```sql
-- Create application-specific user with minimal privileges
CREATE USER twenty_user WITH ENCRYPTED PASSWORD 'strong_random_password_here';

-- Create database
CREATE DATABASE twenty_db OWNER twenty_user;

-- Connect to application database
\c twenty_db;

-- Grant minimal required privileges
GRANT CONNECT ON DATABASE twenty_db TO twenty_user;
GRANT USAGE ON SCHEMA public TO twenty_user;
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO twenty_user;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO twenty_user;

-- Set default privileges for future tables
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO twenty_user;
ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT USAGE, SELECT ON SEQUENCES TO twenty_user;

-- Revoke dangerous privileges
REVOKE CREATE ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA information_schema FROM twenty_user;
REVOKE ALL ON SCHEMA pg_catalog FROM twenty_user;
```

### 8. Data Encryption

#### 8.1 Encryption at Rest
**Priority:** High  
**Implementation Timeline:** Week 1  

```hcl
# RDS with encryption
resource "aws_db_instance" "twenty_db" {
  identifier = "twenty-crm-db"
  
  engine         = "postgres"
  engine_version = "16.0"
  instance_class = "db.t3.micro"
  
  allocated_storage     = 20
  max_allocated_storage = 100
  storage_type         = "gp3"
  storage_encrypted    = true
  kms_key_id          = aws_kms_key.twenty_db_key.arn
  
  db_name  = var.database_name
  username = var.database_username
  password = var.database_password
  
  vpc_security_group_ids = [aws_security_group.db_sg.id]
  db_subnet_group_name   = aws_db_subnet_group.twenty_db.name
  
  backup_retention_period = 7
  backup_window          = "03:00-04:00"
  maintenance_window     = "sun:04:00-sun:05:00"
  
  skip_final_snapshot = false
  final_snapshot_identifier = "twenty-db-final-snapshot"
  
  enabled_cloudwatch_logs_exports = ["postgresql"]
  
  tags = {
    Name = "twenty-crm-database"
  }
}

# KMS key for database encryption
resource "aws_kms_key" "twenty_db_key" {
  description             = "KMS key for Twenty CRM database encryption"
  deletion_window_in_days = 7
  enable_key_rotation     = true
  
  tags = {
    Name = "twenty-db-encryption-key"
  }
}
```

#### 8.2 Application-Level Encryption
**Priority:** Medium  
**Implementation Timeline:** Week 2  

```javascript
const crypto = require('crypto');

class EncryptionService {
  constructor() {
    this.algorithm = 'aes-256-gcm';
    this.key = Buffer.from(process.env.ENCRYPTION_KEY, 'hex'); // 32 bytes
  }
  
  encrypt(text) {
    const iv = crypto.randomBytes(16);
    const cipher = crypto.createCipher(this.algorithm, this.key);
    cipher.setAAD(Buffer.from('twenty-crm', 'utf8'));
    
    let encrypted = cipher.update(text, 'utf8', 'hex');
    encrypted += cipher.final('hex');
    
    const authTag = cipher.getAuthTag();
    
    return {
      encrypted,
      iv: iv.toString('hex'),
      authTag: authTag.toString('hex')
    };
  }
  
  decrypt(encryptedData) {
    const decipher = crypto.createDecipher(this.algorithm, this.key);
    decipher.setAAD(Buffer.from('twenty-crm', 'utf8'));
    decipher.setAuthTag(Buffer.from(encryptedData.authTag, 'hex'));
    
    let decrypted = decipher.update(encryptedData.encrypted, 'hex', 'utf8');
    decrypted += decipher.final('utf8');
    
    return decrypted;
  }
}

// Usage for sensitive fields
const encryptionService = new EncryptionService();

// Encrypt before saving to database
const encryptedSSN = encryptionService.encrypt(user.ssn);
user.ssn_encrypted = JSON.stringify(encryptedSSN);
```

---

## Monitoring and Logging

### 9. Security Monitoring

#### 9.1 Application Logging
**Priority:** High  
**Implementation Timeline:** Week 1  

```javascript
const winston = require('winston');
const { ElasticsearchTransport } = require('winston-elasticsearch');

// Security-focused logging configuration
const securityLogger = winston.createLogger({
  level: 'info',
  format: winston.format.combine(
    winston.format.timestamp(),
    winston.format.errors({ stack: true }),
    winston.format.json()
  ),
  defaultMeta: { service: 'twenty-crm-security' },
  transports: [
    new winston.transports.File({ 
      filename: 'logs/security.log',
      level: 'warn'
    }),
    new winston.transports.File({ 
      filename: 'logs/combined.log' 
    }),
    new ElasticsearchTransport({
      level: 'info',
      clientOpts: { node: process.env.ELASTICSEARCH_URL },
      index: 'twenty-crm-security'
    })
  ]
});

// Security event logging middleware
const logSecurityEvent = (eventType, details) => {
  securityLogger.warn({
    event_type: eventType,
    timestamp: new Date().toISOString(),
    user_id: details.userId || 'anonymous',
    ip_address: details.ip,
    user_agent: details.userAgent,
    details: details.additional || {}
  });
};

// Authentication logging
const authLogger = (req, res, next) => {
  res.on('finish', () => {
    if (req.path.includes('/auth') || req.path.includes('/login')) {
      logSecurityEvent('auth_attempt', {
        userId: req.user?.id,
        ip: req.ip,
        userAgent: req.get('User-Agent'),
        success: res.statusCode < 400,
        statusCode: res.statusCode,
        additional: { path: req.path, method: req.method }
      });
    }
  });
  next();
};
```

#### 9.2 Intrusion Detection
**Priority:** Medium  
**Implementation Timeline:** Week 2  

```javascript
const rateLimit = require('express-rate-limit');
const slowDown = require('express-slow-down');

// Rate limiting with progressive delays
const authLimiter = rateLimit({
  windowMs: 15 * 60 * 1000, // 15 minutes
  max: 5, // Limit each IP to 5 requests per windowMs
  message: {
    error: 'Too many authentication attempts, please try again later'
  },
  standardHeaders: true,
  legacyHeaders: false,
  // Store in Redis for distributed systems
  store: new RedisStore({
    sendCommand: (...args) => redisClient.call(...args),
  }),
  // Skip successful requests
  skipSuccessfulRequests: true,
  // Custom handler for rate limit exceeded
  handler: (req, res) => {
    logSecurityEvent('rate_limit_exceeded', {
      ip: req.ip,
      userAgent: req.get('User-Agent'),
      path: req.path
    });
    res.status(429).json({ error: 'Rate limit exceeded' });
  }
});

// Progressive delay for suspicious activity
const speedLimiter = slowDown({
  windowMs: 15 * 60 * 1000, // 15 minutes
  delayAfter: 2, // Allow 2 requests per windowMs without delay
  delayMs: 500, // Add 500ms delay per request after delayAfter
  maxDelayMs: 20000, // Maximum delay of 20 seconds
});

// Suspicious activity detection
const suspiciousActivityDetector = (req, res, next) => {
  const suspiciousPatterns = [
    /(\bor\b|\band\b).*?=.*?(\bor\b|\band\b)/i, // SQL injection patterns
    /<script\b[^<]*(?:(?!<\/script>)<[^<]*)*<\/script>/gi, // XSS patterns
    /\.\.\/|\.\.\\/gi, // Path traversal
    /eval\s*\(/gi, // Code injection
  ];
  
  const checkSuspicious = (str) => {
    return suspiciousPatterns.some(pattern => pattern.test(str));
  };
  
  // Check query parameters, body, and headers
  const allInput = JSON.stringify({
    query: req.query,
    body: req.body,
    headers: req.headers
  });
  
  if (checkSuspicious(allInput)) {
    logSecurityEvent('suspicious_activity', {
      ip: req.ip,
      userAgent: req.get('User-Agent'),
      path: req.path,
      method: req.method,
      additional: { input: allInput }
    });
    
    return res.status(400).json({ error: 'Request blocked' });
  }
  
  next();
};

// Apply security middleware
app.use('/auth', authLimiter, speedLimiter);
app.use(suspiciousActivityDetector);
```

### 10. AWS Security Services Integration

#### 10.1 CloudTrail Configuration
**Priority:** High  
**Implementation Timeline:** Week 1  

```hcl
resource "aws_cloudtrail" "twenty_trail" {
  name                          = "twenty-crm-trail"
  s3_bucket_name               = aws_s3_bucket.twenty_logs.bucket
  s3_key_prefix                = "cloudtrail"
  include_global_service_events = true
  is_multi_region_trail        = true
  enable_logging               = true
  
  enable_log_file_validation = true
  kms_key_id                = aws_kms_key.twenty_logs_key.arn
  
  event_selector {
    read_write_type                 = "All"
    include_management_events       = true
    exclude_management_event_sources = []
    
    data_resource {
      type   = "AWS::S3::Object"
      values = ["${aws_s3_bucket.twenty_logs.arn}/*"]
    }
  }
  
  tags = {
    Name = "twenty-crm-cloudtrail"
  }
}
```

#### 10.2 GuardDuty Integration
**Priority:** Medium  
**Implementation Timeline:** Week 2  

```hcl
resource "aws_guardduty_detector" "twenty_guardduty" {
  enable = true
  
  datasources {
    s3_logs {
      enable = true
    }
    kubernetes {
      audit_logs {
        enable = true
      }
    }
    malware_protection {
      scan_ec2_instance_with_findings {
        ebs_volumes {
          enable = true
        }
      }
    }
  }
  
  tags = {
    Name = "twenty-crm-guardduty"
  }
}

# GuardDuty findings to SNS for alerting
resource "aws_sns_topic" "security_alerts" {
  name = "twenty-security-alerts"
  
  kms_master_key_id = aws_kms_key.twenty_sns_key.id
}

resource "aws_cloudwatch_event_rule" "guardduty_finding" {
  name        = "guardduty-finding"
  description = "Capture GuardDuty findings"
  
  event_pattern = jsonencode({
    source      = ["aws.guardduty"]
    detail-type = ["GuardDuty Finding"]
  })
}

resource "aws_cloudwatch_event_target" "sns" {
  rule      = aws_cloudwatch_event_rule.guardduty_finding.name
  target_id = "SendToSNS"
  arn       = aws_sns_topic.security_alerts.arn
}
```

---

## Container Security

### 11. Docker Security Hardening

#### 11.1 Secure Dockerfile
**Priority:** High  
**Implementation Timeline:** Week 1  

```dockerfile
# Use specific version, not latest
FROM node:18.17.0-alpine3.18

# Create non-root user
RUN addgroup -g 1001 -S twenty && \
    adduser -S twenty -u 1001 -G twenty

# Set secure working directory
WORKDIR /app

# Copy package files first for better caching
COPY package*.json ./
COPY yarn.lock ./

# Install dependencies as root, then switch user
RUN yarn install --frozen-lockfile --production && \
    yarn cache clean && \
    npm cache clean --force

# Copy source code
COPY --chown=twenty:twenty . .

# Remove development dependencies
RUN yarn install --production --frozen-lockfile && \
    rm -rf node_modules/.cache

# Switch to non-root user
USER twenty

# Expose port
EXPOSE 3000

# Health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:3000/healthz || exit 1

# Use specific entrypoint
ENTRYPOINT ["node", "dist/main.js"]
```

#### 11.2 Docker Compose Security
**Priority:** High  
**Implementation Timeline:** Week 1  

```yaml
version: '3.8'

services:
  twenty-server:
    build: 
      context: .
      dockerfile: Dockerfile.prod
    
    # Security constraints
    read_only: true
    cap_drop:
      - ALL
    cap_add:
      - NET_BIND_SERVICE
    
    # Resource limits
    deploy:
      resources:
        limits:
          memory: 1G
          cpus: '0.5'
        reservations:
          memory: 512M
          cpus: '0.25'
    
    # Security options
    security_opt:
      - no-new-privileges:true
      - apparmor:docker-default
    
    # Temporary filesystem for writable areas
    tmpfs:
      - /tmp:noexec,nosuid,size=1G
      - /var/cache:noexec,nosuid,size=100M
    
    # Environment variables from secrets
    environment:
      - NODE_ENV=production
    env_file:
      - .env.production
    
    # Networks
    networks:
      - frontend
      - backend
    
    # No external ports (use reverse proxy)
    expose:
      - "3000"
    
    # Health check
    healthcheck:
      test: ["CMD", "curl", "-f", "http://localhost:3000/healthz"]
      interval: 30s
      timeout: 10s
      retries: 3
      start_period: 40s

networks:
  frontend:
    driver: bridge
    internal: false
  backend:
    driver: bridge
    internal: true

# Use named volumes for persistence
volumes:
  twenty-data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: /opt/twenty/data
```

---

## Environment and Configuration Security

### 12. Environment Variables Management

#### 12.1 AWS Secrets Manager Integration
**Priority:** High  
**Implementation Timeline:** Week 1  

```javascript
const AWS = require('aws-sdk');

class SecretsManager {
  constructor() {
    this.client = new AWS.SecretsManager({
      region: process.env.AWS_REGION || 'us-east-1'
    });
  }
  
  async getSecret(secretName) {
    try {
      const data = await this.client.getSecretValue({
        SecretId: secretName
      }).promise();
      
      if (data.SecretString) {
        return JSON.parse(data.SecretString);
      }
      
      return Buffer.from(data.SecretBinary, 'base64').toString('ascii');
    } catch (error) {
      console.error('Error retrieving secret:', error);
      throw error;
    }
  }
  
  async updateSecret(secretName, secretValue) {
    try {
      await this.client.updateSecret({
        SecretId: secretName,
        SecretString: JSON.stringify(secretValue)
      }).promise();
    } catch (error) {
      console.error('Error updating secret:', error);
      throw error;
    }
  }
}

// Configuration loader with secrets
class ConfigLoader {
  constructor() {
    this.secretsManager = new SecretsManager();
    this.config = {};
  }
  
  async loadConfig() {
    try {
      // Load database credentials from secrets
      const dbSecrets = await this.secretsManager.getSecret('twenty-crm/database');
      
      // Load JWT secrets
      const jwtSecrets = await this.secretsManager.getSecret('twenty-crm/jwt');
      
      // Load third-party API keys
      const apiSecrets = await this.secretsManager.getSecret('twenty-crm/api-keys');
      
      this.config = {
        database: {
          host: process.env.DB_HOST,
          port: process.env.DB_PORT || 5432,
          database: process.env.DB_NAME,
          username: dbSecrets.username,
          password: dbSecrets.password
        },
        jwt: {
          secret: jwtSecrets.secret,
          refreshSecret: jwtSecrets.refreshSecret
        },
        apis: {
          googleClientId: apiSecrets.googleClientId,
          googleClientSecret: apiSecrets.googleClientSecret,
          microsoftClientId: apiSecrets.microsoftClientId,
          microsoftClientSecret: apiSecrets.microsoftClientSecret
        },
        encryption: {
          key: await this.secretsManager.getSecret('twenty-crm/encryption-key')
        }
      };
      
      return this.config;
    } catch (error) {
      console.error('Failed to load configuration:', error);
      process.exit(1);
    }
  }
}

// Usage in application startup
const configLoader = new ConfigLoader();
configLoader.loadConfig().then(config => {
  // Start application with secure configuration
  startApplication(config);
});
```

#### 12.2 Terraform Secrets Management
**Priority:** High  
**Implementation Timeline:** Week 1  

```hcl
# Secrets Manager resources
resource "aws_secretsmanager_secret" "twenty_db_credentials" {
  name                    = "twenty-crm/database"
  description             = "Database credentials for Twenty CRM"
  recovery_window_in_days = 7
  kms_key_id             = aws_kms_key.twenty_secrets_key.arn
  
  tags = {
    Name = "twenty-db-credentials"
  }
}

resource "aws_secretsmanager_secret_version" "twenty_db_credentials" {
  secret_id = aws_secretsmanager_secret.twenty_db_credentials.id
  secret_string = jsonencode({
    username = var.database_username
    password = var.database_password
  })
}

# JWT secrets
resource "aws_secretsmanager_secret" "twenty_jwt_secrets" {
  name                    = "twenty-crm/jwt"
  description             = "JWT secrets for Twenty CRM"
  recovery_window_in_days = 7
  kms_key_id             = aws_kms_key.twenty_secrets_key.arn
}

resource "random_password" "jwt_secret" {
  length  = 64
  special = true
}

resource "random_password" "jwt_refresh_secret" {
  length  = 64
  special = true
}

resource "aws_secretsmanager_secret_version" "twenty_jwt_secrets" {
  secret_id = aws_secretsmanager_secret.twenty_jwt_secrets.id
  secret_string = jsonencode({
    secret        = random_password.jwt_secret.result
    refreshSecret = random_password.jwt_refresh_secret.result
  })
}

# KMS key for secrets encryption
resource "aws_kms_key" "twenty_secrets_key" {
  description             = "KMS key for Twenty CRM secrets encryption"
  deletion_window_in_days = 7
  enable_key_rotation     = true
  
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Sid    = "Enable IAM User Permissions"
        Effect = "Allow"
        Principal = {
          AWS = "arn:aws:iam::${data.aws_caller_identity.current.account_id}:root"
        }
        Action   = "kms:*"
        Resource = "*"
      },
      {
        Sid    = "Allow Twenty CRM to use the key"
        Effect = "Allow"
        Principal = {
          AWS = aws_iam_role.twenty_task_role.arn
        }
        Action = [
          "kms:Decrypt",
          "kms:DescribeKey"
        ]
        Resource = "*"
      }
    ]
  })
  
  tags = {
    Name = "twenty-secrets-encryption-key"
  }
}
```

---

## Compliance and Governance

### 13. Security Compliance Framework

#### 13.1 GDPR Compliance Implementation
**Priority:** High  
**Implementation Timeline:** Week 2  

```javascript
// GDPR compliance middleware
const gdprCompliance = {
  // Data retention policies
  dataRetention: {
    userProfiles: 2555, // 7 years in days
    auditLogs: 2555,
    backups: 365,
    tempFiles: 30
  },
  
  // Consent management
  consentTypes: {
    NECESSARY: 'necessary',
    ANALYTICS: 'analytics', 
    MARKETING: 'marketing',
    PERSONALIZATION: 'personalization'
  },
  
  // Data processing lawful bases
  lawfulBases: {
    CONSENT: 'consent',
    CONTRACT: 'contract',
    LEGAL_OBLIGATION: 'legal_obligation',
    VITAL_INTERESTS: 'vital_interests',
    PUBLIC_TASK: 'public_task',
    LEGITIMATE_INTERESTS: 'legitimate_interests'
  }
};

// GDPR data processing logger
class GDPRLogger {
  static logDataProcessing(userId, operation, dataType, lawfulBasis) {
    const logEntry = {
      timestamp: new Date().toISOString(),
      userId,
      operation, // CREATE, READ, UPDATE, DELETE
      dataType, // PERSONAL, SENSITIVE, etc.
      lawfulBasis,
      processingPurpose: this.getProcessingPurpose(operation),
      retention: gdprCompliance.dataRetention[dataType] || 365
    };
    
    // Log to GDPR audit trail
    gdprAuditLogger.info('data_processing', logEntry);
  }
  
  static getProcessingPurpose(operation) {
    const purposes = {
      CREATE: 'user_registration',
      READ: 'service_provision',
      UPDATE: 'data_accuracy',
      DELETE: 'data_erasure'
    };
    return purposes[operation] || 'unknown';
  }
}

// Data subject rights implementation
class DataSubjectRights {
  async handleDataRequest(userId, requestType) {
    switch (requestType) {
      case 'ACCESS':
        return await this.exportUserData(userId);
      case 'RECTIFICATION':
        return await this.updateUserData(userId);
      case 'ERASURE':
        return await this.eraseUserData(userId);
      case 'PORTABILITY':
        return await this.exportPortableData(userId);
      case 'RESTRICTION':
        return await this.restrictProcessing(userId);
      case 'OBJECTION':
        return await this.handleObjection(userId);
      default:
        throw new Error('Invalid request type');
    }
  }
  
  async exportUserData(userId) {
    const userData = await userRepository.findOne({
      where: { id: userId },
      relations: ['companies', 'contacts', 'activities']
    });
    
    // Remove sensitive system data
    const exportData = {
      profile: {
        id: userData.id,
        email: userData.email,
        firstName: userData.firstName,
        lastName: userData.lastName,
        createdAt: userData.createdAt,
        updatedAt: userData.updatedAt
      },
      companies: userData.companies.map(company => ({
        name: company.name,
        website: company.website,
        industry: company.industry
      })),
      contacts: userData.contacts.map(contact => ({
        firstName: contact.firstName,
        lastName: contact.lastName,
        email: contact.email,
        phone: contact.phone
      }))
    };
    
    GDPRLogger.logDataProcessing(userId, 'READ', 'PERSONAL', 'CONSENT');
    return exportData;
  }
  
  async eraseUserData(userId) {
    const transaction = await getConnection().transaction();
    
    try {
      // Anonymize instead of delete where legally required
      await transaction.update(User, userId, {
        email: `anonymized_${userId}@deleted.local`,
        firstName: 'Deleted',
        lastName: 'User',
        phone: null,
        avatar: null,
        deletedAt: new Date()
      });
      
      // Delete or anonymize related data
      await transaction.delete(Contact, { userId });
      await transaction.update(Activity, { userId }, { userId: null });
      
      await transaction.commit();
      
      GDPRLogger.logDataProcessing(userId, 'DELETE', 'PERSONAL', 'CONSENT');
      return { success: true, message: 'User data erased successfully' };
    } catch (error) {
      await transaction.rollback();
      throw error;
    }
  }
}
```

#### 13.2 SOC 2 Controls Implementation
**Priority:** Medium  
**Implementation Timeline:** Week 3  

```javascript
// SOC 2 compliance controls
class SOC2Controls {
  // Security controls
  static async implementAccessControls() {
    // Principle of least privilege
    const roleBasedAccess = {
      admin: ['users:*', 'companies:*', 'settings:*', 'audit:read'],
      manager: ['companies:*', 'contacts:*', 'reports:read'],
      user: ['companies:read', 'contacts:read', 'own_profile:*']
    };
    
    return roleBasedAccess;
  }
  
  // Availability controls
  static async implementBackupProcedures() {
    const backupConfig = {
      frequency: 'daily',
      retention: 30, // days
      offsite: true,
      encryption: true,
      testing: 'weekly'
    };
    
    return backupConfig;
  }
  
  // Processing integrity controls
  static async implementDataValidation() {
    const validationRules = {
      inputValidation: true,
      outputValidation: true,
      dataIntegrityChecks: true,
      transactionLogging: true
    };
    
    return validationRules;
  }
  
  // Confidentiality controls
  static async implementEncryption() {
    const encryptionConfig = {
      dataAtRest: 'AES-256',
      dataInTransit: 'TLS 1.3',
      keyManagement: 'AWS KMS',
      keyRotation: 'annual'
    };
    
    return encryptionConfig;
  }
  
  // Privacy controls
  static async implementPrivacyControls() {
    const privacyConfig = {
      dataClassification: true,
      accessLogging: true,
      dataMinimization: true,
      consentManagement: true,
      dataRetention: true
    };
    
    return privacyConfig;
  }
}
```

---

## Implementation Timeline and Priorities

### Phase 1: Critical Security Fixes (Week 1)
- [ ] Disable GraphQL introspection
- [ ] Implement security headers with Helmet.js
- [ ] Fix CORS configuration
- [ ] Add proper authentication middleware
- [ ] Remove server information disclosure
- [ ] Implement basic input validation

### Phase 2: Authentication and Authorization (Week 2)
- [ ] Enhanced JWT security configuration
- [ ] Role-based access control (RBAC)
- [ ] Multi-factor authentication (MFA)
- [ ] Session management improvements
- [ ] Password policy enforcement

### Phase 3: Infrastructure Security (Week 3)
- [ ] Network segmentation
- [ ] Security groups configuration
- [ ] Database security hardening
- [ ] Encryption implementation
- [ ] WAF deployment

### Phase 4: Monitoring and Compliance (Week 4)
- [ ] Security logging implementation
- [ ] Intrusion detection setup
- [ ] AWS security services integration
- [ ] GDPR compliance features
- [ ] SOC 2 controls implementation

### Phase 5: Advanced Security Features (Week 5-6)
- [ ] Container security hardening
- [ ] Secrets management implementation
- [ ] Advanced threat detection
- [ ] Security testing automation
- [ ] Incident response procedures

---

## Security Testing and Validation

### Automated Security Testing
```bash
#!/bin/bash
# Security testing pipeline

# Static code analysis
npm audit --audit-level moderate
eslint . --ext .js,.ts --config .eslintrc-security.js

# Dependency vulnerability scanning
snyk test

# Container security scanning
docker run --rm -v /var/run/docker.sock:/var/run/docker.sock \
  -v $PWD:/path \
  aquasec/trivy image twentycrm/twenty:latest

# OWASP ZAP automated scanning
docker run -t owasp/zap2docker-stable zap-baseline.py \
  -t http://localhost:3000 \
  -J zap-report.json

# Infrastructure security testing
terraform plan -out=tfplan
checkov -f tfplan --framework terraform
```

### Manual Security Testing Checklist
- [ ] Authentication bypass testing
- [ ] Authorization testing
- [ ] Input validation testing
- [ ] Session management testing
- [ ] Error handling testing
- [ ] Business logic testing
- [ ] API security testing
- [ ] Infrastructure penetration testing

---

## Incident Response Plan

### Security Incident Classification
1. **Critical (P0):** Active breach, data theft, complete system compromise
2. **High (P1):** Unauthorized access, privilege escalation, data exposure
3. **Medium (P2):** Successful reconnaissance, failed intrusion attempts
4. **Low (P3):** Policy violations, suspicious activity

### Response Procedures
1. **Detection and Analysis**
   - Monitor security alerts
   - Analyze log files
   - Validate incidents

2. **Containment**
   - Isolate affected systems
   - Block malicious traffic
   - Preserve evidence

3. **Eradication**
   - Remove threats
   - Update security controls
   - Patch vulnerabilities

4. **Recovery**
   - Restore services
   - Monitor for reoccurrence
   - Update security measures

5. **Post-Incident**
   - Document lessons learned
   - Update procedures
   - Improve detection capabilities

---

## Conclusion

This comprehensive security hardening guide provides the foundation for securing the Twenty CRM application against current and emerging threats. Implementation should follow the phased approach, prioritizing critical vulnerabilities first.

Regular security assessments, continuous monitoring, and staying updated with security best practices are essential for maintaining a strong security posture.

**Next Steps:**
1. Begin with Phase 1 critical fixes
2. Establish security monitoring
3. Implement automated security testing
4. Regular security reviews and updates
5. Staff security training and awareness

**Success Metrics:**
- Zero critical vulnerabilities
- 99.9% uptime
- Compliance with security frameworks
- Reduced security incidents
- Improved security awareness

### Documentation References
- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework)
- [AWS Security Best Practices](https://aws.amazon.com/security/security-resources/)
- [Twenty CRM Documentation](https://twenty.com/developers)

### Vulnerability Databases
- [CVE Details](https://www.cvedetails.com/)
- [NIST NVD](https://nvd.nist.gov/)
- [OWASP WebGoat](https://owasp.org/www-project-webgoat/)
