terraform {
  required_version = ">= 1.0"
  required_providers {
    aws = {
      source  = "hashicorp/aws"
      version = "~> 5.0"
    }
  }
}

provider "aws" {
  region = var.aws_region
}

# Data sources
data "aws_availability_zones" "available" {
  state = "available"
}

data "aws_caller_identity" "current" {}

# VPC Module
module "vpc" {
  source = "./modules/vpc"

  project_name       = var.project_name
  environment        = var.environment
  availability_zones = data.aws_availability_zones.available.names
  vpc_cidr          = var.vpc_cidr
  public_subnets    = var.public_subnets
  private_subnets   = var.private_subnets
  database_subnets  = var.database_subnets

  tags = local.common_tags
}

# Security Module
module "security" {
  source = "./modules/security"

  project_name = var.project_name
  environment  = var.environment
  vpc_id       = module.vpc.vpc_id
  vpc_cidr     = var.vpc_cidr

  tags = local.common_tags
}

# RDS Module
module "rds" {
  source = "./modules/rds"

  project_name        = var.project_name
  environment         = var.environment
  vpc_id              = module.vpc.vpc_id
  database_subnets    = module.vpc.database_subnets
  security_group_ids  = [module.security.database_security_group_id]
  
  db_instance_class   = var.db_instance_class
  db_allocated_storage = var.db_allocated_storage
  db_engine_version   = var.db_engine_version
  
  kms_key_id          = module.security.kms_key_id

  tags = local.common_tags
}

# ALB Module
module "alb" {
  source = "./modules/alb"

  project_name        = var.project_name
  environment         = var.environment
  vpc_id              = module.vpc.vpc_id
  public_subnets      = module.vpc.public_subnets
  security_group_ids  = [module.security.alb_security_group_id]
  
  certificate_arn     = var.certificate_arn
  domain_name         = var.domain_name

  tags = local.common_tags
}

# ECS Module
module "ecs" {
  source = "./modules/ecs"

  project_name           = var.project_name
  environment            = var.environment
  vpc_id                 = module.vpc.vpc_id
  private_subnets        = module.vpc.private_subnets
  security_group_ids     = [module.security.ecs_security_group_id]
  
  target_group_arn       = module.alb.target_group_arn
  task_cpu               = var.task_cpu
  task_memory            = var.task_memory
  
  # Database connection
  database_host          = module.rds.db_endpoint
  database_port          = module.rds.db_port
  database_name          = module.rds.db_name
  database_secret_arn    = module.rds.db_secret_arn
  
  # KMS key for encryption
  kms_key_id             = module.security.kms_key_id

  tags = local.common_tags
}

# Local values
locals {
  common_tags = {
    Project     = var.project_name
    Environment = var.environment
    ManagedBy   = "terraform"
    Owner       = "security-team"
  }
} 