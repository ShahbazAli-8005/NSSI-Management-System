import smtplib
from email.mime.text import MIMEText

# 1. Read email list from file
with open("selectedemails.txt", "r") as f:
    recipients = [line.strip() for line in f if line.strip()]

# 2. Email content
subject = "Blood Donation Request"
body = """Dear donor,

We are reaching out to ask for your support in donating blood.

Your donation can save lives. Please consider visiting the nearest donation center.

Thank you!
"""

# 3. Set your Gmail credentials
sender_email = "iamtayyabshahzad2025@gmail.com"
app_password = "cnji evzc wuvr tdel"

# 4. Setup email server
server = smtplib.SMTP_SSL("smtp.gmail.com", 465)
server.login(sender_email, app_password)

# 5. Send emails
for email in recipients:
    msg = MIMEText(body)
    msg["Subject"] = subject
    msg["From"] = sender_email
    msg["To"] = email
    server.sendmail(sender_email, email, msg.as_string())

server.quit()
print("✅ All emails sent successfully.")
