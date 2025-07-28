# Twenty CRM Security Challenge - Deployment Guide

## Overview

This guide provides step-by-step instructions for deploying the Twenty CRM Security Challenge infrastructure and conducting penetration testing assessment.

## Prerequisites

### Required Tools

- **AWS CLI** (v2.0+)
- **Terraform** (v1.0+)
- **Docker** (v20.0+)
- **Docker Compose** (v2.0+)
- **Python 3** (v3.8+)
- **jq** (for JSON processing)
- **OpenSSL** (for certificate generation)

### AWS Requirements

- AWS Account with appropriate permissions
- Configured AWS CLI (`aws configure`)
- IAM permissions for:
  - EC2 (VPC, Security Groups, Load Balancer)
  - RDS (PostgreSQL instances)
  - KMS (Key management)
  - Secrets Manager
  - CloudWatch (Logs and metrics)
  - WAF (Web Application Firewall)
  - GuardDuty and Security Hub

## Architecture

The deployment creates a secure, multi-tier architecture:

```
Internet → CloudFront → ALB → ECS Tasks → RDS
                   ↓
                  WAF
                   ↓
             CloudWatch Logs
```

### Components

1. **VPC**: Secure network with public/private subnets
2. **ALB**: Application Load Balancer with SSL termination
3. **ECS**: Container orchestration for Twenty CRM
4. **RDS**: PostgreSQL database with encryption
5. **WAF**: Web Application Firewall protection
6. **Security**: KMS encryption, Security Groups, GuardDuty
7. **Monitoring**: CloudWatch logs, metrics, and alarms

## Quick Start

### 1. Clone and Navigate

```bash
cd wp-projects/resola
```

### 2. Configure AWS

```bash
# Configure AWS CLI
aws configure

# Verify configuration
aws sts get-caller-identity
```

### 3. Deploy Infrastructure

```bash
# Run automated deployment
./scripts/deploy.sh

# Or deploy components separately
./scripts/deploy.sh --skip-application  # Infrastructure only
./scripts/deploy.sh --skip-infrastructure  # Application only
```

### 4. Verify Deployment

```bash
# Check application status
cd docker
docker-compose ps

# Check application logs
docker-compose logs -f twenty-server
```

## Manual Deployment

### Step 1: Infrastructure Deployment

```bash
# Navigate to Terraform directory
cd terraform

# Initialize Terraform
terraform init

# Create terraform.tfvars
cat > terraform.tfvars << EOF
project_name = "twenty-crm-security"
environment = "dev"
aws_region = "us-east-1"
domain_name = "twenty-crm.local"
enable_waf = true
EOF

# Plan and apply
terraform plan -out=tfplan
terraform apply tfplan

# Save outputs
terraform output -json > ../outputs.json
```

### Step 2: Application Configuration

```bash
# Navigate back to root
cd ..

# Copy environment template
cp docker/env.template docker/.env

# Update .env with your values
# - Generate secrets: openssl rand -base64 32
# - Update database connection from Terraform outputs
# - Configure email settings
```

### Step 3: Application Deployment

```bash
# Navigate to Docker directory
cd docker

# Create required directories
sudo mkdir -p /opt/twenty/{data/{server,worker,redis},logs/{server,worker}}
sudo chown -R $(whoami):$(whoami) /opt/twenty

# Deploy application
docker-compose pull
docker-compose up -d

# Verify deployment
docker-compose ps
```

## Security Configuration

### SSL/TLS Setup

```bash
# Generate self-signed certificate (development)
openssl genrsa -out certificates/app.key 2048
openssl req -new -key certificates/app.key -out certificates/app.csr
openssl x509 -req -days 365 -in certificates/app.csr -signkey certificates/app.key -out certificates/app.crt

# For production, use ACM or upload valid certificates
```

### Database Security

- Encryption at rest using KMS
- Encryption in transit using SSL
- Network isolation in private subnets
- Secrets stored in AWS Secrets Manager

### Application Security

- WAF protection with managed rules
- Rate limiting
- Security headers (HSTS, CSP, etc.)
- Container security hardening
- Network segmentation

## Penetration Testing

### Setup Testing Environment

```bash
# Navigate to pentest directory
cd pentest

# Create virtual environment
python3 -m venv venv
source venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

### Automated Testing

```bash
# Run comprehensive security assessment
python3 pentest_runner.py https://your-domain.com -u test@example.com -p TestPassword123!

# Run specific test categories
python3 pentest_runner.py https://your-domain.com --test auth
python3 pentest_runner.py https://your-domain.com --test input
python3 pentest_runner.py https://your-domain.com --test api
```

### Manual Testing Tools

#### Burp Suite

```bash
# Import configuration
# File → Project options → Load from file → burp-suite-config.json

# Configure proxy
# Proxy → Options → Proxy Listeners → Add → 8080

# Start testing
# Target → Site map → Right-click → Scan
```

#### OWASP ZAP

```bash
# Start ZAP
zap.sh -daemon -config api.disablekey=true -port 8080

# Run automated scan
zap-cli quick-scan https://your-domain.com
```

### Testing Checklist

- [ ] Information disclosure
- [ ] Authentication bypass
- [ ] Authorization vulnerabilities
- [ ] Input validation (XSS, SQL injection)
- [ ] Session management
- [ ] File upload vulnerabilities
- [ ] API security
- [ ] CSRF protection
- [ ] Rate limiting
- [ ] Security headers

## Monitoring and Logging

### CloudWatch

```bash
# View application logs
aws logs tail /aws/ecs/twenty-crm-security-dev --follow

# View WAF logs
aws logs tail /aws/waf/twenty-crm-security-dev --follow

# View VPC Flow Logs
aws logs tail /aws/vpc/flow-logs/twenty-crm-security-dev --follow
```

### Application Monitoring

```bash
# Docker logs
docker-compose logs -f

# Database monitoring
aws rds describe-db-instances --db-instance-identifier twenty-crm-security-dev-db

# ECS service status
aws ecs describe-services --cluster twenty-crm-security-dev --services twenty-crm-security-dev-service
```

## Troubleshooting

### Common Issues

#### Database Connection Issues

```bash
# Check database status
aws rds describe-db-instances --db-instance-identifier twenty-crm-security-dev-db

# Test database connectivity
psql -h db-endpoint -U username -d twentycrm

# Check secrets
aws secretsmanager get-secret-value --secret-id twenty-crm-security-dev-db-credentials
```

#### Application Startup Issues

```bash
# Check container logs
docker-compose logs twenty-server
docker-compose logs twenty-worker

# Check resource usage
docker stats

# Restart services
docker-compose restart
```

#### WAF Blocking Issues

```bash
# Check WAF logs
aws logs filter-log-events --log-group-name /aws/waf/twenty-crm-security-dev

# Temporarily disable WAF rules
aws wafv2 update-web-acl --scope REGIONAL --id <web-acl-id> --default-action Allow={}
```

### Health Checks

```bash
# Application health
curl -k https://your-domain.com/healthz

# Database health
docker-compose exec twenty-server sh -c 'curl -f http://localhost:3000/healthz'

# Load balancer health
aws elbv2 describe-target-health --target-group-arn <target-group-arn>
```

## Security Best Practices

### Development Environment

- Use self-signed certificates for testing
- Enable debug logging for troubleshooting
- Use development-specific secrets
- Implement basic WAF rules

### Production Environment

- Use valid SSL certificates (ACM)
- Disable debug logging
- Use strong, unique secrets
- Enable comprehensive WAF rules
- Implement backup strategies
- Set up alerting and monitoring

## Performance Optimization

### Database Optimization

```sql
-- Enable query performance insights
-- Configure parameter groups for optimal performance
-- Set up read replicas for read-heavy workloads
```

### Application Optimization

```bash
# Optimize Docker images
docker-compose build --no-cache

# Configure resource limits
# Update docker-compose.yml with appropriate CPU/memory limits

# Enable application-level caching
# Configure Redis for session storage and caching
```

## Backup and Recovery

### Database Backups

```bash
# Create manual backup
aws rds create-db-snapshot --db-instance-identifier twenty-crm-security-dev-db --db-snapshot-identifier manual-backup-$(date +%Y%m%d)

# Restore from backup
aws rds restore-db-instance-from-db-snapshot --db-instance-identifier restored-db --db-snapshot-identifier manual-backup-20241201
```

### Application Backups

```bash
# Backup application data
docker-compose exec twenty-server tar -czf /backup/app-data-$(date +%Y%m%d).tar.gz /app/data

# Backup configuration
cp -r docker/.env backups/
cp -r terraform/terraform.tfstate backups/
```

## Cleanup

### Destroy Infrastructure

```bash
# Stop application
docker-compose down

# Destroy Terraform infrastructure
cd terraform
terraform destroy

# Clean up Docker resources
docker system prune -a
```

### Remove Data

```bash
# Remove application data
sudo rm -rf /opt/twenty

# Remove certificates
rm -rf certificates/

# Remove logs
rm -rf logs/
```

## Support and References

### Documentation

- [Twenty CRM Documentation](https://twenty.com/developers)
- [AWS Security Best Practices](https://docs.aws.amazon.com/security/)
- [OWASP Testing Guide](https://owasp.org/www-project-web-security-testing-guide/)

### Security Resources

- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [AWS Security Hub](https://aws.amazon.com/security-hub/)
- [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework)

### Tools

- [Burp Suite](https://portswigger.net/burp)
- [OWASP ZAP](https://www.zaproxy.org/)
- [Nmap](https://nmap.org/)
- [SQLMap](https://sqlmap.org/)

## Conclusion

This deployment guide provides comprehensive instructions for setting up and securing the Twenty CRM application for security assessment purposes. The infrastructure implements multiple layers of security controls while maintaining observability and monitoring capabilities.

Remember to follow security best practices, regularly update dependencies, and conduct periodic security assessments to maintain a strong security posture. 