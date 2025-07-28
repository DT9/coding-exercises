#!/bin/bash

# Twenty CRM Security Challenge Deployment Script
# This script deploys the complete infrastructure for the security assessment

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
PROJECT_NAME="twenty-crm-security"
ENVIRONMENT="dev"
AWS_REGION="us-east-1"
TERRAFORM_DIR="terraform"
DOCKER_DIR="docker"
SCRIPTS_DIR="scripts"

# Functions
log() {
    echo -e "${GREEN}[$(date +'%Y-%m-%d %H:%M:%S')] $1${NC}"
}

warn() {
    echo -e "${YELLOW}[$(date +'%Y-%m-%d %H:%M:%S')] WARNING: $1${NC}"
}

error() {
    echo -e "${RED}[$(date +'%Y-%m-%d %H:%M:%S')] ERROR: $1${NC}"
    exit 1
}

info() {
    echo -e "${BLUE}[$(date +'%Y-%m-%d %H:%M:%S')] INFO: $1${NC}"
}

check_requirements() {
    log "Checking requirements..."
    
    # Check if AWS CLI is installed and configured
    if ! command -v aws &> /dev/null; then
        error "AWS CLI is not installed. Please install it first."
    fi
    
    # Check AWS credentials
    if ! aws sts get-caller-identity &> /dev/null; then
        error "AWS credentials not configured. Please run 'aws configure' first."
    fi
    
    # Check if Terraform is installed
    if ! command -v terraform &> /dev/null; then
        error "Terraform is not installed. Please install it first."
    fi
    
    # Check if Docker is installed
    if ! command -v docker &> /dev/null; then
        error "Docker is not installed. Please install it first."
    fi
    
    # Check if Docker Compose is installed
    if ! command -v docker-compose &> /dev/null; then
        error "Docker Compose is not installed. Please install it first."
    fi
    
    # Check if Python 3 is installed
    if ! command -v python3 &> /dev/null; then
        error "Python 3 is not installed. Please install it first."
    fi
    
    log "All requirements satisfied."
}

setup_environment() {
    log "Setting up environment..."
    
    # Create necessary directories
    mkdir -p logs
    mkdir -p backups
    mkdir -p certificates
    
    # Create .env file if it doesn't exist
    if [ ! -f "${DOCKER_DIR}/.env" ]; then
        log "Creating .env file from template..."
        cp "${DOCKER_DIR}/env.template" "${DOCKER_DIR}/.env"
        warn "Please update the .env file with your actual configuration values"
    fi
    
    # Generate secrets if they don't exist
    if ! grep -q "your-very-secure-app-secret-key-here" "${DOCKER_DIR}/.env"; then
        log "Generating secure secrets..."
        
        # Generate strong secrets
        APP_SECRET=$(openssl rand -base64 32)
        ACCESS_TOKEN_SECRET=$(openssl rand -base64 32)
        LOGIN_TOKEN_SECRET=$(openssl rand -base64 32)
        REFRESH_TOKEN_SECRET=$(openssl rand -base64 32)
        FILE_TOKEN_SECRET=$(openssl rand -base64 32)
        REDIS_PASSWORD=$(openssl rand -base64 16)
        
        # Update .env file
        sed -i "s/your-very-secure-app-secret-key-here/${APP_SECRET}/g" "${DOCKER_DIR}/.env"
        sed -i "s/your-access-token-secret-here/${ACCESS_TOKEN_SECRET}/g" "${DOCKER_DIR}/.env"
        sed -i "s/your-login-token-secret-here/${LOGIN_TOKEN_SECRET}/g" "${DOCKER_DIR}/.env"
        sed -i "s/your-refresh-token-secret-here/${REFRESH_TOKEN_SECRET}/g" "${DOCKER_DIR}/.env"
        sed -i "s/your-file-token-secret-here/${FILE_TOKEN_SECRET}/g" "${DOCKER_DIR}/.env"
        sed -i "s/your-redis-password/${REDIS_PASSWORD}/g" "${DOCKER_DIR}/.env"
        
        log "Secrets generated and updated in .env file"
    fi
    
    log "Environment setup complete."
}

deploy_infrastructure() {
    log "Deploying AWS infrastructure..."
    
    cd "${TERRAFORM_DIR}"
    
    # Initialize Terraform
    log "Initializing Terraform..."
    terraform init
    
    # Create terraform.tfvars if it doesn't exist
    if [ ! -f "terraform.tfvars" ]; then
        log "Creating terraform.tfvars..."
        cat > terraform.tfvars << EOF
project_name = "${PROJECT_NAME}"
environment = "${ENVIRONMENT}"
aws_region = "${AWS_REGION}"
domain_name = "twenty-crm.local"
enable_waf = true
EOF
    fi
    
    # Plan deployment
    log "Planning Terraform deployment..."
    terraform plan -out=tfplan
    
    # Apply deployment
    log "Applying Terraform deployment..."
    terraform apply tfplan
    
    # Save outputs
    log "Saving Terraform outputs..."
    terraform output -json > ../outputs.json
    
    cd ..
    
    log "Infrastructure deployment complete."
}

configure_application() {
    log "Configuring application..."
    
    # Check if outputs exist
    if [ ! -f "outputs.json" ]; then
        error "Terraform outputs not found. Please run infrastructure deployment first."
    fi
    
    # Extract values from Terraform outputs
    ALB_DNS_NAME=$(cat outputs.json | jq -r '.alb_dns_name.value')
    DB_ENDPOINT=$(cat outputs.json | jq -r '.database_endpoint.value')
    DB_PORT=$(cat outputs.json | jq -r '.database_port.value')
    DB_NAME=$(cat outputs.json | jq -r '.database_name.value')
    DB_SECRET_ARN=$(cat outputs.json | jq -r '.database_secret_arn.value')
    
    # Get database credentials from AWS Secrets Manager
    log "Retrieving database credentials..."
    DB_CREDENTIALS=$(aws secretsmanager get-secret-value --secret-id "${DB_SECRET_ARN}" --query 'SecretString' --output text)
    DB_USERNAME=$(echo "${DB_CREDENTIALS}" | jq -r '.username')
    DB_PASSWORD=$(echo "${DB_CREDENTIALS}" | jq -r '.password')
    
    # Update .env file with infrastructure values
    sed -i "s/your-domain.com/${ALB_DNS_NAME}/g" "${DOCKER_DIR}/.env"
    sed -i "s/rds-endpoint/${DB_ENDPOINT}/g" "${DOCKER_DIR}/.env"
    sed -i "s/username:password/${DB_USERNAME}:${DB_PASSWORD}/g" "${DOCKER_DIR}/.env"
    sed -i "s/your-postgres-admin-password/${DB_PASSWORD}/g" "${DOCKER_DIR}/.env"
    
    log "Application configuration complete."
}

deploy_application() {
    log "Deploying application..."
    
    cd "${DOCKER_DIR}"
    
    # Create required directories on host
    sudo mkdir -p /opt/twenty/{data/{server,worker,redis},logs/{server,worker}}
    sudo chown -R $(whoami):$(whoami) /opt/twenty
    
    # Pull latest images
    log "Pulling Docker images..."
    docker-compose pull
    
    # Start services
    log "Starting application services..."
    docker-compose up -d
    
    # Wait for services to be ready
    log "Waiting for services to be ready..."
    sleep 30
    
    # Check if services are running
    if docker-compose ps | grep -q "Up"; then
        log "Application services started successfully."
    else
        error "Failed to start application services."
    fi
    
    cd ..
    
    log "Application deployment complete."
}

setup_monitoring() {
    log "Setting up monitoring..."
    
    # Create CloudWatch dashboard
    aws cloudwatch put-dashboard \
        --dashboard-name "${PROJECT_NAME}-${ENVIRONMENT}" \
        --dashboard-body file://scripts/dashboard.json
    
    # Create CloudWatch alarms
    aws cloudwatch put-metric-alarm \
        --alarm-name "${PROJECT_NAME}-${ENVIRONMENT}-high-cpu" \
        --alarm-description "High CPU utilization" \
        --metric-name CPUUtilization \
        --namespace AWS/ECS \
        --statistic Average \
        --period 300 \
        --threshold 80 \
        --comparison-operator GreaterThanThreshold \
        --evaluation-periods 2
    
    log "Monitoring setup complete."
}

setup_security_testing() {
    log "Setting up security testing environment..."
    
    # Install Python dependencies for penetration testing
    if [ ! -f "pentest/requirements.txt" ]; then
        cat > pentest/requirements.txt << EOF
requests==2.28.2
urllib3==1.26.15
python-nmap==0.7.1
dnspython==2.3.0
beautifulsoup4==4.12.2
lxml==4.9.2
paramiko==3.1.0
cryptography==40.0.2
EOF
    fi
    
    # Create virtual environment
    if [ ! -d "pentest/venv" ]; then
        log "Creating Python virtual environment..."
        python3 -m venv pentest/venv
        source pentest/venv/bin/activate
        pip install -r pentest/requirements.txt
        deactivate
    fi
    
    # Make penetration testing script executable
    chmod +x pentest/pentest_runner.py
    
    # Create test user in the application
    log "Creating test user for security testing..."
    cd "${DOCKER_DIR}"
    
    # Wait for application to be ready
    sleep 10
    
    # Create test user via API (if available)
    # This would need to be customized based on Twenty CRM's user creation API
    
    cd ..
    
    log "Security testing setup complete."
}

create_ssl_certificate() {
    log "Creating self-signed SSL certificate..."
    
    # Create certificate directory
    mkdir -p certificates
    
    # Generate private key
    openssl genrsa -out certificates/app.key 2048
    
    # Generate certificate signing request
    openssl req -new -key certificates/app.key -out certificates/app.csr -subj "/C=US/ST=State/L=City/O=Organization/CN=twenty-crm.local"
    
    # Generate self-signed certificate
    openssl x509 -req -days 365 -in certificates/app.csr -signkey certificates/app.key -out certificates/app.crt
    
    log "SSL certificate created."
}

backup_configuration() {
    log "Creating configuration backup..."
    
    # Create backup directory
    mkdir -p backups/$(date +%Y%m%d_%H%M%S)
    BACKUP_DIR="backups/$(date +%Y%m%d_%H%M%S)"
    
    # Backup Terraform state
    cp -r "${TERRAFORM_DIR}/.terraform" "${BACKUP_DIR}/"
    cp "${TERRAFORM_DIR}/terraform.tfstate" "${BACKUP_DIR}/" 2>/dev/null || true
    cp "${TERRAFORM_DIR}/terraform.tfvars" "${BACKUP_DIR}/" 2>/dev/null || true
    
    # Backup Docker configuration
    cp "${DOCKER_DIR}/.env" "${BACKUP_DIR}/" 2>/dev/null || true
    cp "${DOCKER_DIR}/docker-compose.yml" "${BACKUP_DIR}/"
    
    # Backup certificates
    cp -r certificates "${BACKUP_DIR}/" 2>/dev/null || true
    
    log "Configuration backup created in ${BACKUP_DIR}"
}

print_summary() {
    log "Deployment Summary"
    echo "==================="
    
    if [ -f "outputs.json" ]; then
        ALB_DNS_NAME=$(cat outputs.json | jq -r '.alb_dns_name.value')
        echo "Application URL: https://${ALB_DNS_NAME}"
        echo "Database Endpoint: $(cat outputs.json | jq -r '.database_endpoint.value')"
        echo "VPC ID: $(cat outputs.json | jq -r '.vpc_id.value')"
        echo "KMS Key ID: $(cat outputs.json | jq -r '.kms_key_id.value')"
    fi
    
    echo ""
    echo "Next Steps:"
    echo "1. Update DNS records to point to the ALB DNS name"
    echo "2. Update SSL certificates if using custom domain"
    echo "3. Configure email settings in .env file"
    echo "4. Run penetration testing: cd pentest && python3 pentest_runner.py https://your-domain.com"
    echo "5. Review CloudWatch logs and metrics"
    echo ""
    echo "Security Testing:"
    echo "- Burp Suite configuration: pentest/burp-suite-config.json"
    echo "- Automated testing: pentest/pentest_runner.py"
    echo "- OWASP ZAP configuration: pentest/zap-config.json"
    echo ""
    echo "Monitoring:"
    echo "- CloudWatch Dashboard: https://console.aws.amazon.com/cloudwatch/home?region=${AWS_REGION}#dashboards:name=${PROJECT_NAME}-${ENVIRONMENT}"
    echo "- Application logs: docker-compose logs -f"
}

# Main deployment function
main() {
    log "Starting Twenty CRM Security Challenge deployment..."
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --skip-infrastructure)
                SKIP_INFRASTRUCTURE=true
                shift
                ;;
            --skip-application)
                SKIP_APPLICATION=true
                shift
                ;;
            --environment)
                ENVIRONMENT="$2"
                shift 2
                ;;
            --region)
                AWS_REGION="$2"
                shift 2
                ;;
            --help)
                echo "Usage: $0 [OPTIONS]"
                echo "Options:"
                echo "  --skip-infrastructure    Skip Terraform infrastructure deployment"
                echo "  --skip-application      Skip Docker application deployment"
                echo "  --environment ENV       Set environment (default: dev)"
                echo "  --region REGION         Set AWS region (default: us-east-1)"
                echo "  --help                  Show this help message"
                exit 0
                ;;
            *)
                error "Unknown option: $1"
                ;;
        esac
    done
    
    # Check requirements
    check_requirements
    
    # Setup environment
    setup_environment
    
    # Create SSL certificate
    create_ssl_certificate
    
    # Deploy infrastructure (if not skipped)
    if [ "$SKIP_INFRASTRUCTURE" != "true" ]; then
        deploy_infrastructure
    fi
    
    # Configure application
    configure_application
    
    # Deploy application (if not skipped)
    if [ "$SKIP_APPLICATION" != "true" ]; then
        deploy_application
    fi
    
    # Setup monitoring
    setup_monitoring
    
    # Setup security testing
    setup_security_testing
    
    # Create backup
    backup_configuration
    
    # Print summary
    print_summary
    
    log "Deployment complete!"
}

# Error handling
trap 'error "Deployment failed at line $LINENO"' ERR

# Run main function
main "$@" 