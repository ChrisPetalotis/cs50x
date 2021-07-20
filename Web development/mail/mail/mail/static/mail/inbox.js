document.addEventListener('DOMContentLoaded', function() {

  // Use buttons to toggle between views
  document.querySelector('#inbox').addEventListener('click', () => load_mailbox('inbox'));
  document.querySelector('#sent').addEventListener('click', () => load_mailbox('sent'));
  document.querySelector('#archived').addEventListener('click', () => load_mailbox('archive'));
  document.querySelector('#compose').addEventListener('click', compose_email);

  document.querySelector('input[type="submit"]').onclick = (e) => send_email(e);

  // By default, load the inbox
  load_mailbox('inbox');
});

function send_email(e) {
  e.preventDefault();
  document.querySelector('#error-message').innerHTML = '';

  const recipients = document.querySelector('#compose-recipients').value;
  const subject = document.querySelector('#compose-subject').value;
  const body = document.querySelector('#compose-body').value;
  
  // Send POST request to emails API
  fetch('/emails', {
    method: 'POST',
    body: JSON.stringify({
      recipients,
      subject,
      body
    })
  })
  .then(response => response.json())
  .then(result =>  {
    if (result.error) {
      document.querySelector('#error-message').innerHTML = result.error;
      return;
    }

    // Once the email has been sent, load the user's sent mailbox
    load_mailbox('sent');
  });
}

function compose_email(email=null) {

  // Show compose view and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#single-email-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'block';

  if (email) {
    document.querySelector('#compose-recipients').value = email.sender;
    document.querySelector('#compose-subject').value = email.subject.includes("Re: ") ? email.subject : `Re: ${email.subject}`;
    document.querySelector('#compose-body').value = `On ${email.timestamp} ${email.sender} wrote:\n\n` + email.body;
  } else {
    // Clear out composition fields
    document.querySelector('#compose-recipients').value = '';
    document.querySelector('#compose-subject').value = '';
    document.querySelector('#compose-body').value = '';
  }

  document.querySelector('#error-message').innerHTML = '';
}

function load_mailbox(mailbox) {
  
  // Show the mailbox and hide other views
  document.querySelector('#emails-view').style.display = 'block';
  document.querySelector('#compose-view').style.display = 'none';
  document.querySelector('#single-email-view').style.display = 'none';

  // Show the mailbox name
  document.querySelector('#emails-view').innerHTML = `<h3>${mailbox.charAt(0).toUpperCase() + mailbox.slice(1)}</h3>`;

  // Query emails from API
  fetch(`/emails/${mailbox}`)
    .then(response => response.json())
    .then(emails => {
      for (let email of emails) {
        const div = document.createElement('div');
        div.classList.add('email-entry');
        div.innerHTML = `
            <div class="email-contents">
              <div class="email-sender">from: ${email.sender}</div>
              <div class="email-subject">${email.subject}</div>
              <div class="email-timestamp">${email.timestamp}</div>
            </div>
        `;

        div.onclick = () => {
          // Mark email as read, if not already marked so
          if (!email.read) {
            fetch(`/emails/${email.id}`, {
              method: 'PUT',
              body: JSON.stringify({
                read: true
              })
            });
          }

          // Show email's contents
          load_mail_page(email.id, mailbox);
        }
        document.querySelector('#emails-view').append(div);

        if (email.read) {
          div.style.backgroundColor = '	#D3D3D3';
        } else {
          div.style.backgroundColor = 'white';
        }
      }
    })
}

function load_mail_page(email_id, source_page=null) {

  // Show the single email view and hide other views
  document.querySelector('#emails-view').style.display = 'none';
  document.querySelector('#compose-view').style.display = 'none';
  document.querySelector('#single-email-view').style.display = 'block';
  document.querySelector('#single-email-view').innerHTML = '';

  fetch(`emails/${email_id}`)
    .then(response => response.json())
    .then(email => {
      const div = document.createElement('div');
      // div.classList.add('email-header');
      div.innerHTML = `
        <div class="email-header">
          <div class="people">
            <div class="sender"><strong>From</strong>: ${email.sender}</div>
            <div class="recipients"><strong>To</strong>: ${email.recipients.toString()}</div>
          </div>
          <div class="subject">${email.subject}</div>
          <div class="timestamp">${email.timestamp}</div>    
        </div>
        <div>
          <button class="btn btn-sm btn-outline-primary" id="reply-email">
            Reply
          </button>
          <button class="btn btn-sm btn-outline-primary ${(source_page === 'sent' || (email.archived && source_page !== 'archive')) ? 'hide_archive' : ''}" id="archive-email">
          ${source_page === 'inbox' ? 'Archive' : (source_page === 'archive' ? 'Unarchive' : '')}
          </button>
        </div>
        <div class="body">${email.body}</div>
      `;
      document.querySelector('#single-email-view').append(div);
      document.querySelector('#archive-email').onclick = () => {
        archive(email.id, email.archived)
      };
      document.querySelector('#reply-email').onclick = () => {
        compose_email(email);
      };
    }); 
}

function archive(email_id, email_archived_status) {
  console.log(email_archived_status);
  
  // Update email's archived status
  fetch(`emails/${email_id}`, {
    method: 'PUT',
    body: JSON.stringify({
      archived: !email_archived_status
    })
  });

  // Once an email has been archived or unarchived, load the user's inbox
  load_mailbox('inbox');
}