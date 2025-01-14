/*
 * Copyright (c) 2023 Attila Szakacs
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "syslog-ng-otlp-dest-worker.hpp"

using namespace syslogng::grpc::otel;
using namespace opentelemetry::proto::logs::v1;

LogThreadedDestWorker *
SyslogNgDestWorker::construct(LogThreadedDestDriver *o, gint worker_index)
{
  SyslogNgOtlpDestWorker *self = g_new0(SyslogNgOtlpDestWorker, 1);

  otel_dw_init_super(&self->super, o, worker_index);
  self->cpp = new SyslogNgDestWorker(self);

  return &self->super;
}

ScopeLogs *
SyslogNgDestWorker::lookup_scope_logs(LogMessage *msg)
{
  if (logs_service_request.resource_logs_size() > 0)
    return logs_service_request.mutable_resource_logs(0)->mutable_scope_logs(0);

  clear_current_msg_metadata();
  formatter.get_metadata_for_syslog_ng(current_msg_metadata.resource, current_msg_metadata.resource_schema_url,
                                       current_msg_metadata.scope, current_msg_metadata.scope_schema_url);

  ResourceLogs *resource_logs = logs_service_request.add_resource_logs();
  resource_logs->mutable_resource()->CopyFrom(current_msg_metadata.resource);
  resource_logs->set_schema_url(current_msg_metadata.resource_schema_url);

  ScopeLogs *scope_logs = resource_logs->add_scope_logs();
  scope_logs->mutable_scope()->CopyFrom(current_msg_metadata.scope);
  scope_logs->set_schema_url(current_msg_metadata.scope_schema_url);

  return scope_logs;
}

LogThreadedResult
SyslogNgDestWorker::insert(LogMessage *msg)
{
  ScopeLogs *scope_logs = lookup_scope_logs(msg);
  LogRecord *log_record = scope_logs->add_log_records();
  formatter.format_syslog_ng(msg, *log_record);

  return LTR_QUEUED;
}
