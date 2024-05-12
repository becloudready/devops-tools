# Azure Bicep Templates Repository

This repository contains a collection of Azure Bicep templates designed to streamline the deployment of various popular applications and services in Azure, including MySQL, PostgreSQL, SQL Server, and Kubernetes clusters. These templates provide a fast, consistent, and reliable method to deploy infrastructure as code, allowing for easy integration into DevOps pipelines.

## Prerequisites

Before you can deploy these templates, ensure you meet the following prerequisites:
- Azure CLI installed on your machine ([Install Azure CLI](https://docs.microsoft.com/en-us/cli/azure/install-azure-cli))
- An active Azure subscription ([Create an Azure Account](https://azure.microsoft.com/en-us/free/))
- Logged into Azure CLI (`az login`)

## Getting Started


To get started with these Bicep templates, clone this repository to your local machine:

```bash
git clone git@github.com:becloudready/devops-tools.git
cd devops-tools
```

Move to right folder

```
cd devops-tools/azure-bicep-templates

```

## Repository Structure
The repository is organized into directories based on the service type, each containing relevant Bicep templates and a README for detailed instructions:

- /mysql - Templates for deploying Azure MySQL Flexible Servers
- /postgresql - Templates for deploying Azure PostgreSQL Servers
- /sqlserver - Templates for deploying Azure SQL Databases
- /kubernetes - Templates for setting up Azure Kubernetes Service (AKS)

## Contributing
Contributions to this repository are welcome! To contribute:

