# VPC Outputs
output "vpc_id" {
  description = "ID of the VPC"
  value       = module.vpc.vpc_id
}

output "public_subnets" {
  description = "List of public subnet IDs"
  value       = module.vpc.public_subnets
}

output "private_subnets" {
  description = "List of private subnet IDs"
  value       = module.vpc.private_subnets
}

output "database_subnets" {
  description = "List of database subnet IDs"
  value       = module.vpc.database_subnets
}

# Security Outputs
output "alb_security_group_id" {
  description = "ID of the ALB security group"
  value       = module.security.alb_security_group_id
}

output "ecs_security_group_id" {
  description = "ID of the ECS security group"
  value       = module.security.ecs_security_group_id
}

output "database_security_group_id" {
  description = "ID of the database security group"
  value       = module.security.database_security_group_id
}

output "kms_key_id" {
  description = "ID of the KMS key"
  value       = module.security.kms_key_id
}

output "kms_key_arn" {
  description = "ARN of the KMS key"
  value       = module.security.kms_key_arn
}

output "waf_web_acl_arn" {
  description = "ARN of the WAF Web ACL"
  value       = module.security.waf_web_acl_arn
}

# Database Outputs
output "database_endpoint" {
  description = "RDS instance endpoint"
  value       = module.rds.db_endpoint
}

output "database_port" {
  description = "RDS instance port"
  value       = module.rds.db_port
}

output "database_name" {
  description = "RDS database name"
  value       = module.rds.db_name
}

output "database_secret_arn" {
  description = "ARN of the database secret in Secrets Manager"
  value       = module.rds.db_secret_arn
  sensitive   = true
}

# Load Balancer Outputs
output "alb_dns_name" {
  description = "DNS name of the load balancer"
  value       = module.alb.alb_dns_name
}

output "alb_zone_id" {
  description = "Zone ID of the load balancer"
  value       = module.alb.alb_zone_id
}

output "alb_arn" {
  description = "ARN of the load balancer"
  value       = module.alb.alb_arn
}

output "target_group_arn" {
  description = "ARN of the target group"
  value       = module.alb.target_group_arn
}

# ECS Outputs
output "ecs_cluster_name" {
  description = "Name of the ECS cluster"
  value       = module.ecs.cluster_name
}

output "ecs_service_name" {
  description = "Name of the ECS service"
  value       = module.ecs.service_name
}

output "ecs_task_definition_arn" {
  description = "ARN of the ECS task definition"
  value       = module.ecs.task_definition_arn
}

output "ecs_task_role_arn" {
  description = "ARN of the ECS task role"
  value       = module.ecs.task_role_arn
}

output "ecs_execution_role_arn" {
  description = "ARN of the ECS execution role"
  value       = module.ecs.execution_role_arn
}

# Application Outputs
output "application_url" {
  description = "URL of the Twenty CRM application"
  value       = "https://${module.alb.alb_dns_name}"
}

output "cloudwatch_log_group" {
  description = "CloudWatch log group for the application"
  value       = module.ecs.log_group_name
}

# Monitoring Outputs
output "cloudwatch_dashboard_url" {
  description = "URL to the CloudWatch dashboard"
  value       = "https://console.aws.amazon.com/cloudwatch/home?region=${var.aws_region}#dashboards:name=${var.project_name}-${var.environment}"
}

# Security Testing Outputs
output "penetration_testing_info" {
  description = "Information for penetration testing"
  value = {
    target_url                = "https://${module.alb.alb_dns_name}"
    database_endpoint         = module.rds.db_endpoint
    database_port            = module.rds.db_port
    security_groups = {
      alb      = module.security.alb_security_group_id
      ecs      = module.security.ecs_security_group_id
      database = module.security.database_security_group_id
    }
    monitoring = {
      log_group = module.ecs.log_group_name
      region    = var.aws_region
    }
  }
}

# Environment Configuration
output "environment_config" {
  description = "Environment configuration summary"
  value = {
    project_name = var.project_name
    environment  = var.environment
    region       = var.aws_region
    vpc_cidr     = var.vpc_cidr
  }
} 